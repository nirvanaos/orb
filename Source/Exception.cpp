#include <CORBA/SystemException.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Environment_c.h>

namespace CORBA {
namespace Nirvana {

void set_exception (I_ptr <EnvironmentBridge> environment, Exception::Code code, const Char* rep_id, const void* param) NIRVANA_NOEXCEPT
{
	if (environment && RepositoryId::compatible (environment->_epv ().interface_id, Bridge < ::CORBA::Environment>::repository_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*&environment);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void set_exception (I_ptr <EnvironmentBridge> environment, const Exception& e) NIRVANA_NOEXCEPT
{
	set_exception (environment, e.__code (), e._rep_id (), e.__data ());
}

void set_unknown_exception (I_ptr <EnvironmentBridge> environment) NIRVANA_NOEXCEPT
{
	set_exception (environment, SystemException::EC_UNKNOWN, CORBA_REPOSITORY_ID ("UNKNOWN"), nullptr);
}

}
}

