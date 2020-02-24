#include <CORBA/Environment_c.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Exception.h>

namespace CORBA {
namespace Nirvana {

template <>
const Char Bridge < ::CORBA::Environment>::interface_id_[] = CORBA_REPOSITORY_ID (Environment);

}
}
