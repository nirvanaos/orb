#include <CORBA/DynamicServant.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <DynamicServant>::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (DynamicServant);

}
}