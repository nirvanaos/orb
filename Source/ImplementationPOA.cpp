#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <PortableServer::ServantBase>& ServantTraitsPOA::_servant (Bridge <PortableServer::ServantBase>* bridge)
{
	_check_pointer (bridge, Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_.interface);
	ServantPOA <PortableServer::ServantBase>& servant = static_cast <ServantPOA <PortableServer::ServantBase>&> (*bridge);
	servant._check_construct ();
	return servant;
}

}
}