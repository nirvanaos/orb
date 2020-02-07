#include <CORBA/FindInterface.h>
#include <CORBA/RepositoryId.h>
#include <string.h>

namespace CORBA {
namespace Nirvana {

Bridge <Interface>* InterfaceEntry::find (const InterfaceEntry* begin, const InterfaceEntry* end, void* servant, const Char* id)
{
	const InterfaceEntry* ie = nullptr;
	if (!id) // On NULL id return primary interface
		ie = begin;
	else {
		for (const InterfaceEntry* p = begin; p != end; ++p) {
			if (RepositoryId::compatible (p->interface_id, id)) {
				ie = p;
				break;
			}
		}
	}
	if (ie) {
		Bridge <Interface>* itf = (ie->cast) (servant);
		assert (!strcmp (itf->_epv ().interface_id, ie->interface_id));
		return itf;
	} else
		return nullptr;
}

}
}
