#include <CORBA/LocalObject.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge <LocalObject>::interface_id_ [] = CORBA_REPOSITORY_ID (Object);

}

const Char LocalObject::check_interface_id_ [] = CORBA_REPOSITORY_ID (LocalObject);

}