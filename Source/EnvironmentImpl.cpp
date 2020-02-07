#include <CORBA/EnvironmentImpl.h>
#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCode.h>
#include <Nirvana/Memory.h>

namespace CORBA {
namespace Nirvana {

void EnvironmentBase::exception_free ()
{
	if (data_.is_small) {
		TypeCode_ptr tc = ((const Exception*)(data_.small))->__type_code ();
		tc->_destruct (data_.small);
	} else
		delete data_.ptr;
	data_.is_small = 0;
	data_.ptr = nullptr;
}

void EnvironmentBase::check () const
{
	const Exception* ex = exception ();
	if (ex)
		ex->raise ();
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

void EnvironmentBase::exception_set (Long code, const char* rep_id, const void* param, 
	const ExceptionEntry* user_exceptions)
{
	exception_free ();
	if (code > Exception::EC_NO_EXCEPTION && rep_id) {
		try {
			TypeCode_ptr etc;
			if (code >= Exception::EC_SYSTEM_EXCEPTION) {
				etc = SystemException::_get_type_code (rep_id, code);
				assert (etc->_size () <= sizeof (data_.small));
			} else {
				etc = _tc_UNKNOWN;
				if (user_exceptions) {
					for (const ExceptionEntry* p = user_exceptions; p->itf; ++p) {
						TypeCode_ptr tc = *p;
						if (RepositoryId::compatible (tc->id (), rep_id)) {
							etc = tc;
							break;
						}
					}
				}
			}
			set (etc, param);
		} catch (...) {
		}
	}
}

void EnvironmentBase::set (TypeCode_ptr tc, const void* data)
{
	ULong size = tc->_size ();
	void* p;
	if (size <= sizeof (data_.small))
		p = data_.small;
	else
		p = ::Nirvana::g_default_heap->allocate (nullptr, size, 0);
	tc->_copy (p, data);
}

void EnvironmentBase::set (const Exception* ex)
{
	exception_free ();
	if (ex)
		set (ex->__type_code (), ex->__data ());
}

}
}
