#include <CORBA/ServantBasePOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <PortableServer::ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_)
{}

Interface* ServantPOA <PortableServer::ServantBase>::_get_proxy (String_in iid)
{
	_check_construct ();
	return ServantBaseLink::_get_proxy (iid);
}

}
}