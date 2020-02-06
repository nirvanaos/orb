#include <CORBA/EnvironmentImpl.h>
#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

EnvironmentBase::~EnvironmentBase ()
{
	delete exception_;
}

void EnvironmentBase::exception_free ()
{
	delete exception_;
	exception_ = nullptr;
}

void EnvironmentBase::check () const
{
	if (exception_)
		exception_->raise ();
}

const Char* EnvironmentBase::exception_id () const
{
	if (exception_)
		return exception_->_rep_id ();
	else
		return nullptr;
}

const void* EnvironmentBase::exception_value () const
{
	if (exception_)
		return exception_->__data ();
	else
		return nullptr;
}

void EnvironmentBase::exception_set (Long code, const char* rep_id, const void* param, 
	const ExceptionEntry* user_exceptions)
{
	exception_free ();
	if (rep_id) {
		Exception* e = nullptr;
		try {
			if (code >= 0)
				e = SystemException::_create (rep_id, param, code);
			else if (user_exceptions) {
				for (const ExceptionEntry* p = user_exceptions; p->rep_id; ++p)
					if (RepositoryId::compatible (p->rep_id, rep_id)) {
						e = (p->create) (param);
						break;
					}
				if (!e)
					e = new UNKNOWN ();
			}
		} catch (...) {
		}
		exception_ = e;
	}
}

}
}
