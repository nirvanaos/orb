#include <CORBA/RepositoryId.h>
#include <CORBA/CORBA_Environment.h>
#include <string.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge < ::CORBA::Environment>::interface_id_ [] = CORBA_REPOSITORY_ID (Environment);

void BridgeMarshal < ::CORBA::Environment>::set_exception (Long code, const char* rep_id, const void* param)
{
	// Client can pass NULL environment in special cases, so we check this for NULL
	if (this && RepositoryId::compatible (_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*this);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void BridgeMarshal < ::CORBA::Environment>::set_exception (const Exception& e)
{
	set_exception (e.__code (), e._rep_id (), e.__data ());
}

void BridgeMarshal < ::CORBA::Environment>::set_unknown_exception ()
{
	set_exception (0, CORBA_REPOSITORY_ID (UNKNOWN), nullptr);
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
