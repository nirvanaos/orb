#include <CORBA/Object.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <Object>::interface_id_ [] = CORBA_REPOSITORY_ID (Object);

}
}
