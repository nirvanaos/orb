#include "AbstractBase.h"

namespace PortableServer {
namespace Nirvana {

void _check_pointer (const void* p)
{
	if (!p)
		throw Exception (33);	// Invalid pointer
}

void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv)
{
	if (!obj || &obj->_epv () != &epv)
		throw Exception (33);	// Invalid pointer
}

InterfaceVirtual <AbstractBase>& InterfaceVirtual <AbstractBase>::_get_base (Bridge <Interface>* itf)
{
	_check_pointer (itf);
	Bridge <DerivedInterface>* pb = static_cast <Bridge <DerivedInterface>*> (itf);
	_check_pointer (itf, pb->_epv ().interface);
	InterfaceVirtualBase <DerivedInterface>* p = static_cast <InterfaceVirtualBase <DerivedInterface>*> (pb);
	return *p;
}

InterfaceVirtual <AbstractBase>& InterfaceVirtual <AbstractBase>::_get_base (Bridge <AbstractBase>* itf)
{
	_check_pointer (itf);
	InterfaceVirtual <AbstractBase>* p = static_cast <InterfaceVirtual <AbstractBase>*> (itf);
	_check_pointer (itf, p->_epv ().interface);
	return *p;
}

}
}