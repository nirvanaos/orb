#include "ImplementationTied.h"

namespace CORBA {
namespace Nirvana {

class DerivedInterface;

template <>
class Bridge <DerivedInterface> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}
};

}
}

namespace PortableServer {
namespace Nirvana {

InterfaceTied <AbstractBase>& InterfaceTied <AbstractBase>::_get_base (Bridge <Interface>* itf)
{
	_check_pointer (itf);
	Bridge <DerivedInterface>* pb = static_cast <Bridge <DerivedInterface>*> (itf);
	_check_pointer (itf, pb->_epv ().interface);
	InterfaceTiedBase <DerivedInterface>* p = static_cast <InterfaceTiedBase <DerivedInterface>*> (pb);
	return *p;
}

InterfaceTied <AbstractBase>& InterfaceTied <AbstractBase>::_get_base (Bridge <AbstractBase>* itf)
{
	_check_pointer (itf);
	InterfaceTied <AbstractBase>* p = static_cast <InterfaceTied <AbstractBase>*> (itf);
	_check_pointer (itf, p->_epv ().interface);
	return *p;
}

}
}
