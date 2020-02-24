#include <CORBA/AbstractBase.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <AbstractBase>::interface_id_ [] = CORBA_REPOSITORY_ID (AbstractBase);

}
}