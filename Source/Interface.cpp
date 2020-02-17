#include <CORBA/Interface.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Environment.h>

namespace CORBA {
namespace Nirvana {

Interface* interface_duplicate (Interface* itf)
{
	if (itf) {
		Environment env;
		itf = (itf->_epv ().duplicate) (itf, &env);
		env.check ();
	}
	return itf;
}

void interface_release (Interface* itf) NIRVANA_NOEXCEPT
{
	if (itf) {
		try {
			(itf->_epv ().release) (itf);
		} catch (...) {}
	}
}

Interface* Interface::_check (Interface* bridge, const Char* interface_id)
{
	if (bridge) {
		//  _check_pointer (bridge);
		const Char* bridge_id = bridge->_epv ().interface_id;
		if (
			bridge_id != interface_id
			&&
			!RepositoryId::compatible (bridge_id, interface_id)
		)
			::Nirvana::throw_INV_OBJREF ();
	}
	return bridge;
}

}
}
	