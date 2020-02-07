/// Macros to define all system exceptions.
/// Order is significant because exceptions must be sorted by repo id.
#ifndef NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_
#define NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_

/// CORBA system exceptions
#define CORBA_EXCEPTIONS(M)/*
*/ M (BAD_CONTEXT) /* error processing context object
*/ M (BAD_INV_ORDER) /* routine invocations out of order
*/ M (BAD_OPERATION) /* invalid operation
*/ M (BAD_PARAM) /* an invalid parameter was passed
*/ M (BAD_QOS) /* bad quality of service
*/ M (BAD_TYPECODE) /* bad typecode
*/ M (CODESET_INCOMPATIBLE) /* incompatible code set
*/ M (COMM_FAILURE) /* communication failure
*/ M (DATA_CONVERSION) /* data conversion error
*/ M (FREE_MEM) /* cannot free memory
*/ M (IMP_LIMIT) /* violated implementation limit
*/ M (INITIALIZE) /* ORB initialization failure
*/ M (INTERNAL) /* ORB internal error
*/ M (INTF_REPOS) /* error accessing interface repository
*/ M (INVALID_TRANSACTION) /* invalid transaction
*/ M (INV_FLAG) /* invalid flag was specified
*/ M (INV_IDENT) /* invalid identifier syntax
*/ M (INV_OBJREF) /* invalid object reference
*/ M (INV_POLICY) /* invalid policy
*/ M (MARSHAL) /* error marshalling param/result
*/ M (NO_IMPLEMENT) /* operation implementation unavailable
*/ M (NO_MEMORY) /* dynamic memory allocation failure
*/ M (NO_PERMISSION) /* no permission for attempted op.
*/ M (NO_RESOURCES) /* insufficient resources for req.
*/ M (NO_RESPONSE) /* response to req. not yet available
*/ M (OBJECT_NOT_EXIST) /* non-existent object, delete reference
*/ M (OBJ_ADAPTER) /* failure detected by object adapter
*/ M (PERSIST_STORE) /* persistent storage failure
*/ M (REBIND) /* rebind needed
*/ M (TIMEOUT) /* operation timed out
*/ M (TRANSACTION_MODE) /* invalid transaction mode
*/ M (TRANSACTION_REQUIRED) /* transaction required
*/ M (TRANSACTION_ROLLEDBACK) /* transaction rolled back
*/ M (TRANSACTION_UNAVAILABLE) /* no transaction
*/ M (TRANSIENT) /* transient failure - reissue request
*/ M (UNKNOWN) /* the unknown exception
*/

/// Nirvana system exceptions
#define NIRVANA_EXCEPTIONS(M)/*
*/ M (MEM_NOT_ALLOCATED) /* memory is not allocated
*/ M (MEM_NOT_COMMITTED) /* memory is not committed
*/

#define SYSTEM_EXCEPTIONS(M) \
CORBA_EXCEPTIONS (M)\
NIRVANA_EXCEPTIONS (M)

#endif
