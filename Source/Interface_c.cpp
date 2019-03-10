#include <Interface_c.h>
#include <RepositoryId.h>
#include <Environment.h>

namespace CORBA {
namespace Nirvana {

Bridge <Interface>* Interface::_duplicate (Bridge <Interface>* itf)
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

Bridge <Interface>* Interface::adopt (Bridge <Interface>* bridge, const Char* interface_id)
{
	if (bridge) {
		const Char* bridge_id = bridge->_epv ().interface_id;
		if (
			bridge_id != interface_id
			&&
			!RepositoryId::compatible (bridge_id, interface_id)
		)
			throw MARSHAL ();
	}
	return bridge;
}

}
}
	