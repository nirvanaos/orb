#include <CORBA/TypeCode_c.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge <TypeCode>::interface_id_[] = CORBA_REPOSITORY_ID (TypeCode);

}

DEFINE_USER_EXCEPTION (TypeCode::Bounds, CORBA_REPOSITORY_ID (TypeCode / Bounds));

}
