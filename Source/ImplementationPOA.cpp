#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <ServantBase>, ServantBase>::epv_)
{}

Interface* ServantPOA <ServantBase>::_get_proxy ()
{
	_check_construct ();
	return ServantBaseLink::_get_proxy ();
}

ServantPOA <LocalObject>::ServantPOA ()
{
	LocalObjectLink::_construct (this);
}

Interface* ServantPOA <LocalObject>::_get_proxy ()
{
	return LocalObjectLink::_get_proxy ();
}

}
}