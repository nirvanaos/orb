#include <CORBA/FindInterface.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/String.h>

namespace CORBA {
namespace Nirvana {

Interface* InterfaceEntry::find (const InterfaceEntry* begin, const InterfaceEntry* end, void* servant, String_in id)
{
	const InterfaceEntry* ie = 0;
	const String& sid = Type <String>::in (&id);
	if (sid.empty ()) // On NULL id return primary interface
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
		Interface* itf = (ie->cast) (servant);
		assert (!strcmp (itf->_epv ().interface_id, ie->interface_id));
		return itf;
	} else
		return nullptr;
}

}
}
