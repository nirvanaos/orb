#include <CORBA/POA.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge < ::PortableServer::POA>::interface_id_ [] = PORTABLESERVER_REPOSITORY_ID (POA);

}
}