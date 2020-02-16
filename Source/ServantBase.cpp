#include <CORBA/ServantBase.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge < ::PortableServer::ServantBase>::interface_id_ [] = CORBA_REPOSITORY_ID (Object);

}
}

namespace PortableServer {

const ::CORBA::Char ServantBase::check_interface_id_ [] = PORTABLESERVER_REPOSITORY_ID (ServantBase);

}
