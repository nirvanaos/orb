#include <CORBA/ServantBase.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge <ServantBase>::interface_id_ [] = CORBA_REPOSITORY_ID (Object);
const Char ServantBase::check_interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (ServantBase);

}
}
