#include <CORBA/ReferenceCounter.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <ReferenceCounter>::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (ReferenceCounter);

}
}