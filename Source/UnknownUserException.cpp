#include <CORBA/UnknownUserException.h>
#include <Nirvana/OLF.h>

namespace CORBA {

TypeCode_ptr UnknownUserException::__type_code () const NIRVANA_NOEXCEPT { return _tc_UnknownUserException; }
DEFINE_USER_EXC (UnknownUserException);
const Char UnknownUserException::repository_id_ [] = CORBA_REPOSITORY_ID ("UnknownUserException");

}
