#include <CORBA/SystemException.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Environment_c.h>
#include <CORBA/UnknownUserException.h>

namespace CORBA {

namespace Nirvana {

void set_exception (EnvironmentBridge* environment, Long code, const char* rep_id, const void* param) NIRVANA_NOEXCEPT
{
	if (environment && RepositoryId::compatible (environment->_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*environment);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void set_exception (EnvironmentBridge* environment, const Exception& e) NIRVANA_NOEXCEPT
{
	const void* data;
	const UnknownUserException* uue = UnknownUserException::_downcast (&e);
	if (uue)
		data = uue->exception ().data ();
	else
		data = e.__data ();
	set_exception (environment, e.__code (), e._rep_id (), data);
}

void set_unknown_exception (EnvironmentBridge* environment) NIRVANA_NOEXCEPT
{
	set_exception (environment, SystemException::EC_UNKNOWN, CORBA_REPOSITORY_ID ("UNKNOWN"), nullptr);
}

}

}

