#include <CORBA/EnvironmentImpl.h>
#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCode.h>
#include <CORBA/UnknownUserException.h>
#include <Nirvana/Memory.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

void EnvironmentBase::exception_free ()
{
	if (data_.is_small) {
		// Call virtual destructor
		((Exception*)(data_.small))->~Exception ();
	} else
		delete data_.ptr;
	data_.is_small = 0;
	data_.ptr = nullptr;
}

void EnvironmentBase::check () const
{
	const Exception* ex = exception ();
	if (ex)
		ex->_raise ();
}

const Char* EnvironmentBase::exception_id () const
{
	const Exception* ex = exception ();
	if (ex)
		return ex->_rep_id ();
	else
		return nullptr;
}

const void* EnvironmentBase::exception_value () const
{
	const Exception* ex = exception ();
	if (ex)
		return ex->__data ();
	else
		return nullptr;
}

void EnvironmentBase::exception (Exception* ex)
{
	exception_free ();
	data_.ptr = ex;
}

void EnvironmentBase::exception_set (Long code, String_in rep_id, const void* param, 
	const ExceptionEntry* user_exceptions)
{
	exception_free ();
	if (code > Exception::EC_NO_EXCEPTION && !static_cast <const String&> (rep_id).empty ()) {
		if (Exception::EC_USER_EXCEPTION == code && user_exceptions) {
			if (RepositoryId::compatible (UnknownUserException::repository_id_, rep_id) && param) {
				const Any* pa = (const Any*)param;
				TypeCode_ptr tc = pa->type ();
				assert (tc->kind () == tk_except);
				if (tc->kind () == tk_except && set_user (tc->id (), pa->data (), user_exceptions))
					return;
			} else if (set_user (rep_id, param, user_exceptions))
				return;
		}
		const ExceptionEntry* ee = SystemException::_get_exception_entry (rep_id, code);
		assert (ee && ee->size <= sizeof (data_.small));
		set_system (*ee, param);
	}
}

bool EnvironmentBase::set_user (String_in rep_id, const void* param, const ExceptionEntry* user_exceptions)
{
	for (const ExceptionEntry* p = user_exceptions; p->rep_id; ++p) {
		if (RepositoryId::compatible (p->rep_id, rep_id)) {
			set_user (*p, param);
			return true;
		}
	}
	return false;
}

bool EnvironmentBase::set (const ExceptionEntry& ee)
{
	size_t size = ee.size;
	Exception* p;
	if (size <= sizeof (data_.small))
		p = (Exception*)data_.small;
	else {
		try {
			p = (Exception*)::Nirvana::g_memory->allocate (nullptr, size, 0);
		} catch (...) {
			new (data_.small) NO_MEMORY ();
			return false;
		}
	}
	ee.construct (p);
	return true;
}

void EnvironmentBase::set_user (const ExceptionEntry& ee, const void* data)
{
	if (set (ee) && data && ee.size > sizeof (UserException)) {
		Exception& e = *exception ();
		TypeCode_ptr tc = e.__type_code ();
		tc->_copy (e.__data (), data);
	}
}

void EnvironmentBase::set_system (const ExceptionEntry& ee, const void* data)
{
	if (set (ee) && data) {
		SystemException& e = static_cast <SystemException&> (*exception ());
		e.minor (((SystemException::Data*)data)->minor);
		e.completed (((SystemException::Data*)data)->completed);
	}
}

void EnvironmentBase::move_from (EnvironmentBase& src) NIRVANA_NOEXCEPT
{
	exception_free ();
	data_ = src.data_;
	src.data_.reset ();
}

}
}
