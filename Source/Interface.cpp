#include <CORBA/Interface.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Environment.h>
#include <exception>

namespace CORBA {
namespace Nirvana {

Bridge <Interface>* Interface::__duplicate (Bridge <Interface>* itf)
{
	if (itf) {
		Environment env;
		itf = (itf->_epv ().duplicate) (itf, &env);
		env.check ();
	}
	return itf;
}

void Interface::_release (Bridge <Interface>* itf)
{
	if (itf)
		(itf->_epv ().release) (itf);
}

Bridge <Interface>* Interface::unmarshal (Bridge <Interface>* bridge, const Char* interface_id)
{
	if (bridge) {
		const Char* bridge_id = bridge->_epv ().interface_id;
		if (
			bridge_id != interface_id
			&&
			!RepositoryId::compatible (bridge_id, interface_id)
		)
			::Nirvana::throw_MARSHAL ();
	}
	return bridge;
}

bool uncaught_exception ()
{
#ifdef NIRVANA_C17
	return std::uncaught_exceptions () != 0;
#else
	return std::uncaught_exception ();
#endif
}

}
}
	