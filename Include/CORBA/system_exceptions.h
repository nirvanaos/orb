/// Macros to define all system exceptions.
#ifndef NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_
#define NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_

#define CORBA_EXCEPTIONS(M)/*
*/ M (UNKNOWN) /* the unknown exception
*/ M (BAD_PARAM) /* an invalid parameter was passed
*/ M (NO_MEMORY) /* dynamic memory allocation failure
*/ M (IMP_LIMIT) /* violated implementation limit
*/ M (COMM_FAILURE) /* communication failure
*/ M (INV_OBJREF) /* invalid object reference
*/ M (NO_PERMISSION) /* no permission for attempted op.
*/ M (INTERNAL) /* ORB internal error
*/ M (MARSHAL) /* error marshalling param/result
*/ M (INITIALIZE) /* ORB initialization failure
*/ M (NO_IMPLEMENT) /* operation implementation unavailable
*/ M (BAD_TYPECODE) /* bad typecode
*/ M (BAD_OPERATION) /* invalid operation
*/ M (NO_RESOURCES) /* insufficient resources for req.
*/ M (NO_RESPONSE) /* response to req. not yet available
*/ M (PERSIST_STORE) /* persistent storage failure
*/ M (BAD_INV_ORDER) /* routine invocations out of order
*/ M (TRANSIENT) /* transient failure - reissue request
*/ M (FREE_MEM) /* cannot free memory
*/ M (INV_IDENT) /* invalid identifier syntax
*/ M (INV_FLAG) /* invalid flag was specified
*/ M (INTF_REPOS) /* error accessing interface repository
*/ M (BAD_CONTEXT) /* error processing context object
*/ M (OBJ_ADAPTER) /* failure detected by object adapter
*/ M (DATA_CONVERSION) /* data conversion error
*/ M (OBJECT_NOT_EXIST) /* non-existent object, delete reference
*/ M (TRANSACTION_REQUIRED) /* transaction required
*/ M (TRANSACTION_ROLLEDBACK) /* transaction rolled back
*/ M (INVALID_TRANSACTION) /* invalid transaction
*/ M (INV_POLICY) /* invalid policy
*/ M (CODESET_INCOMPATIBLE) /* incompatible code set
*/ M (REBIND) /* rebind needed
*/ M (TIMEOUT) /* operation timed out
*/ M (TRANSACTION_UNAVAILABLE) /* no transaction
*/ M (TRANSACTION_MODE) /* invalid transaction mode
*/ M (BAD_QOS) /* bad quality of service
*/

#define NIRVANA_EXCEPTIONS(M)/*
*/ M (MEM_NOT_COMMITTED) /* memory is not committed
*/ M (MEM_NOT_ALLOCATED) /* memory is not allocated
*/

#define SYSTEM_EXCEPTIONS(M) \
CORBA_EXCEPTIONS (M)\
NIRVANA_EXCEPTIONS (M)

#endif
