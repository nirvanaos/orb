#include <CORBA/exceptions.h>
#include <CORBA/TypeCodeException.h>

#define DEFINE_SYS_EXCEPTION(MID, e)\
const Nirvana::StaticInterface <TypeCode> _tc_##e { STATIC_BRIDGE (Nirvana::TypeCodeException <e>, TypeCode) };\
Long e::__code () const { return EC_##e; }\
const e* e::_downcast (const Exception* ep) { return (ep && (EC_##e == ep->__code ())) ? static_cast <const e*> (ep) : nullptr; }\
TypeCode_ptr e::__type_code () const { return _tc_##e; }\
DEFINE_EXCEPTION(e, MID (e))

#define DEFINE_SYSTEM_EXCEPTION(e) DEFINE_SYS_EXCEPTION(CORBA_REPOSITORY_ID, e)
#define DEFINE_NIRVANA_EXCEPTION(e) DEFINE_SYS_EXCEPTION(NIRVANA_REPOSITORY_ID, e)

namespace CORBA {

DEFINE_SYSTEM_EXCEPTION (UNKNOWN) // the unknown exception
DEFINE_SYSTEM_EXCEPTION (BAD_PARAM) // an invalid parameter was passed
DEFINE_SYSTEM_EXCEPTION (NO_MEMORY) // dynamic memory allocation failure
DEFINE_SYSTEM_EXCEPTION (IMP_LIMIT) // violated implementation limit
DEFINE_SYSTEM_EXCEPTION (COMM_FAILURE) // communication failure
DEFINE_SYSTEM_EXCEPTION (INV_OBJREF) // invalid object reference
DEFINE_SYSTEM_EXCEPTION (NO_PERMISSION) // no permission for attempted op.
DEFINE_SYSTEM_EXCEPTION (INTERNAL) // ORB internal error
DEFINE_SYSTEM_EXCEPTION (MARSHAL) // error marshalling param/result
DEFINE_SYSTEM_EXCEPTION (INITIALIZE) // ORB initialization failure
DEFINE_SYSTEM_EXCEPTION (NO_IMPLEMENT) // operation implementation unavailable
DEFINE_SYSTEM_EXCEPTION (BAD_TYPECODE) // bad typecode
DEFINE_SYSTEM_EXCEPTION (BAD_OPERATION) // invalid operation
DEFINE_SYSTEM_EXCEPTION (NO_RESOURCES) // insufficient resources for req.
DEFINE_SYSTEM_EXCEPTION (NO_RESPONSE) // response to req. not yet available
DEFINE_SYSTEM_EXCEPTION (PERSIST_STORE) // persistent storage failure
DEFINE_SYSTEM_EXCEPTION (BAD_INV_ORDER) // routine invocations out of order
DEFINE_SYSTEM_EXCEPTION (TRANSIENT) // transient failure - reissue request
DEFINE_SYSTEM_EXCEPTION (FREE_MEM) // cannot free memory
DEFINE_SYSTEM_EXCEPTION (INV_IDENT) // invalid identifier syntax
DEFINE_SYSTEM_EXCEPTION (INV_FLAG) // invalid flag was specified
DEFINE_SYSTEM_EXCEPTION (INTF_REPOS) // error accessing interface repository
DEFINE_SYSTEM_EXCEPTION (BAD_CONTEXT) // error processing context object
DEFINE_SYSTEM_EXCEPTION (OBJ_ADAPTER) // failure detected by object adapter
DEFINE_SYSTEM_EXCEPTION (DATA_CONVERSION) // data conversion error
DEFINE_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST) // non-existent object, delete reference
DEFINE_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED) // transaction required
DEFINE_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK) // transaction rolled back
DEFINE_SYSTEM_EXCEPTION (INVALID_TRANSACTION) // invalid transaction
DEFINE_SYSTEM_EXCEPTION (INV_POLICY) // invalid policy
DEFINE_SYSTEM_EXCEPTION (CODESET_INCOMPATIBLE) // incompatible code set
DEFINE_SYSTEM_EXCEPTION (REBIND) // rebind needed
DEFINE_SYSTEM_EXCEPTION (TIMEOUT) // operation timed out
DEFINE_SYSTEM_EXCEPTION (TRANSACTION_UNAVAILABLE) // no transaction
DEFINE_SYSTEM_EXCEPTION (TRANSACTION_MODE) // invalid transaction mode
DEFINE_SYSTEM_EXCEPTION (BAD_QOS) // bad quality of service

DEFINE_NIRVANA_EXCEPTION (MEM_NOT_COMMITTED) // memory is not committed
DEFINE_NIRVANA_EXCEPTION (MEM_NOT_ALLOCATED) // memory is not allocated

}
