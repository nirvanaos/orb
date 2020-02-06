#include <CORBA/Environment_c.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Exception.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge < ::CORBA::Environment>::interface_id_[] = CORBA_REPOSITORY_ID (Environment);

void BridgeMarshal < ::CORBA::Environment>::set_exception (Long code, const char* rep_id, const void* param)
{
	if (RepositoryId::compatible (_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
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

}
}
