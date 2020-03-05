#include <CORBA/exceptions.h>
#include <CORBA/TypeCode.h>

#define DEFINE_SYS_EXCEPTION(MID, e)\
Nirvana::ExceptionEntry _tc_##e { STATIC_BRIDGE (Nirvana::TypeCodeException <e>, TypeCode) };\
Long e::__code () const { return EC_##e; }\
const e* e::_downcast (const Exception* ep) { return (ep && (EC_##e == ep->__code ())) ? static_cast <const e*> (ep) : nullptr; }\
TypeCode_ptr e::__type_code () const { return _tc_##e; }\
DEFINE_EXCEPTION(e, MID (#e))

#define DEFINE_SYSTEM_EXCEPTION(e) DEFINE_SYS_EXCEPTION(CORBA_REPOSITORY_ID, e)
#define DEFINE_NIRVANA_EXCEPTION(e) DEFINE_SYS_EXCEPTION(NIRVANA_REPOSITORY_ID, e)

namespace CORBA {

CORBA_EXCEPTIONS (DEFINE_SYSTEM_EXCEPTION)
NIRVANA_EXCEPTIONS (DEFINE_NIRVANA_EXCEPTION)

}
