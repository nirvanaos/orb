#include <CORBA/ObjectFactory.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <ObjectFactory>::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (ObjectFactory);

}
}