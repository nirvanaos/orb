#include <CORBA/ProxyFactory.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <ProxyFactory>::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (ProxyFactory);

}
}
