#ifndef NIRVANA_ORB_EXCEPTIONS_H_
#define NIRVANA_ORB_EXCEPTIONS_H_

#include "SystemException.h"

#define DECLARE_SYSTEM_EXCEPTION(e) \
extern const Nirvana::StaticInterface <TypeCode> _tc_##e;\
class e : public SystemException {\
public: e () {}\
e (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
e (const Data* data) : SystemException (data) {}\
virtual Long __code () const;\
DECLARE_EXCEPTION(e)\
};

namespace CORBA {

DECLARE_SYSTEM_EXCEPTION (UNKNOWN) // the unknown exception
DECLARE_SYSTEM_EXCEPTION (BAD_PARAM) // an invalid parameter was passed
DECLARE_SYSTEM_EXCEPTION (NO_MEMORY) // dynamic memory allocation failure
DECLARE_SYSTEM_EXCEPTION (IMP_LIMIT) // violated implementation limit
DECLARE_SYSTEM_EXCEPTION (COMM_FAILURE) // communication failure
DECLARE_SYSTEM_EXCEPTION (INV_OBJREF) // invalid object reference
DECLARE_SYSTEM_EXCEPTION (NO_PERMISSION) // no permission for attempted op.
DECLARE_SYSTEM_EXCEPTION (INTERNAL) // ORB internal error
DECLARE_SYSTEM_EXCEPTION (MARSHAL) // error marshalling param/result
DECLARE_SYSTEM_EXCEPTION (INITIALIZE) // ORB initialization failure
DECLARE_SYSTEM_EXCEPTION (NO_IMPLEMENT) // operation implementation unavailable
DECLARE_SYSTEM_EXCEPTION (BAD_TYPECODE) // bad typecode
DECLARE_SYSTEM_EXCEPTION (BAD_OPERATION) // invalid operation
DECLARE_SYSTEM_EXCEPTION (NO_RESOURCES) // insufficient resources for req.
DECLARE_SYSTEM_EXCEPTION (NO_RESPONSE) // response to req. not yet available
DECLARE_SYSTEM_EXCEPTION (PERSIST_STORE) // persistent storage failure
DECLARE_SYSTEM_EXCEPTION (BAD_INV_ORDER) // routine invocations out of order
DECLARE_SYSTEM_EXCEPTION (TRANSIENT) // transient failure - reissue request
DECLARE_SYSTEM_EXCEPTION (FREE_MEM) // cannot free memory
DECLARE_SYSTEM_EXCEPTION (INV_IDENT) // invalid identifier syntax
DECLARE_SYSTEM_EXCEPTION (INV_FLAG) // invalid flag was specified
DECLARE_SYSTEM_EXCEPTION (INTF_REPOS) // error accessing interface repository
DECLARE_SYSTEM_EXCEPTION (BAD_CONTEXT) // error processing context object
DECLARE_SYSTEM_EXCEPTION (OBJ_ADAPTER) // failure detected by object adapter
DECLARE_SYSTEM_EXCEPTION (DATA_CONVERSION) // data conversion error
DECLARE_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST) // non-existent object, delete reference
DECLARE_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED) // transaction required
DECLARE_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK) // transaction rolled back
DECLARE_SYSTEM_EXCEPTION (INVALID_TRANSACTION) // invalid transaction
DECLARE_SYSTEM_EXCEPTION (INV_POLICY) // invalid policy
DECLARE_SYSTEM_EXCEPTION (CODESET_INCOMPATIBLE) // incompatible code set
DECLARE_SYSTEM_EXCEPTION (REBIND) // rebind needed
DECLARE_SYSTEM_EXCEPTION (TIMEOUT) // operation timed out
DECLARE_SYSTEM_EXCEPTION (TRANSACTION_UNAVAILABLE) // no transaction
DECLARE_SYSTEM_EXCEPTION (TRANSACTION_MODE) // invalid transaction mode
DECLARE_SYSTEM_EXCEPTION (BAD_QOS) // bad quality of service

DECLARE_SYSTEM_EXCEPTION (MEM_NOT_COMMITTED) // memory is not committed
DECLARE_SYSTEM_EXCEPTION (MEM_NOT_ALLOCATED) // memory is not allocated

}

#undef DECLARE_SYSTEM_EXCEPTION

#endif
