#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCodeException.h>

namespace CORBA {

using namespace Nirvana;

const ExceptionEntry SystemException::creators_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	EX_TABLE_ENTRY (UNKNOWN), // the unknown exception
	EX_TABLE_ENTRY (BAD_PARAM), // an invalid parameter was passed
	EX_TABLE_ENTRY (NO_MEMORY), // dynamic memory allocation failure
	EX_TABLE_ENTRY (IMP_LIMIT), // violated implementation limit
	EX_TABLE_ENTRY (COMM_FAILURE), // communication failure
	EX_TABLE_ENTRY (INV_OBJREF), // invalid object reference
	EX_TABLE_ENTRY (NO_PERMISSION), // no permission for attempted op.
	EX_TABLE_ENTRY (INTERNAL), // ORB internal error
	EX_TABLE_ENTRY (MARSHAL), // error marshalling param/result
	EX_TABLE_ENTRY (INITIALIZE), // ORB initialization failure
	EX_TABLE_ENTRY (NO_IMPLEMENT), // operation implementation unavailable
	EX_TABLE_ENTRY (BAD_TYPECODE), // bad typecode
	EX_TABLE_ENTRY (BAD_OPERATION), // invalid operation
	EX_TABLE_ENTRY (NO_RESOURCES), // insufficient resources for req.
	EX_TABLE_ENTRY (NO_RESPONSE), // response to req. not yet available
	EX_TABLE_ENTRY (PERSIST_STORE), // persistent storage failure
	EX_TABLE_ENTRY (BAD_INV_ORDER), // routine invocations out of order
	EX_TABLE_ENTRY (TRANSIENT), // transient failure - reissue request
	EX_TABLE_ENTRY (FREE_MEM), // cannot free memory
	EX_TABLE_ENTRY (INV_IDENT), // invalid identifier syntax
	EX_TABLE_ENTRY (INV_FLAG), // invalid flag was specified
	EX_TABLE_ENTRY (INTF_REPOS), // error accessing interface repository
	EX_TABLE_ENTRY (BAD_CONTEXT), // error processing context object
	EX_TABLE_ENTRY (OBJ_ADAPTER), // failure detected by object adapter
	EX_TABLE_ENTRY (DATA_CONVERSION), // data conversion error
	EX_TABLE_ENTRY (OBJECT_NOT_EXIST), // non-existent object, delete reference
	EX_TABLE_ENTRY (TRANSACTION_REQUIRED), // transaction required
	EX_TABLE_ENTRY (TRANSACTION_ROLLEDBACK), // transaction rolled back
	EX_TABLE_ENTRY (INVALID_TRANSACTION), // invalid transaction
	EX_TABLE_ENTRY (INV_POLICY), // invalid policy
	EX_TABLE_ENTRY (CODESET_INCOMPATIBLE), // incompatible code set
	EX_TABLE_ENTRY (REBIND), // rebind needed
	EX_TABLE_ENTRY (TIMEOUT), // operation timed out
	EX_TABLE_ENTRY (TRANSACTION_UNAVAILABLE), // no transaction
	EX_TABLE_ENTRY (TRANSACTION_MODE), // invalid transaction mode
	EX_TABLE_ENTRY (BAD_QOS), // bad quality of service

	EX_TABLE_ENTRY (MEM_NOT_COMMITTED), // memory is not committed
	EX_TABLE_ENTRY (MEM_NOT_ALLOCATED) // memory is not allocated
};

Exception* SystemException::_create (const char* rep_id, const void* data, int hint)
{
	const ExceptionEntry* entry = 0;
	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS && Nirvana::RepositoryId::compatible (creators_ [hint].rep_id, rep_id))
		entry = creators_ + hint;
	else {
		for (const ExceptionEntry* pe = creators_; pe != creators_ + KNOWN_SYSTEM_EXCEPTIONS; ++pe)
			if (Nirvana::RepositoryId::compatible (pe->rep_id, rep_id)) {
				entry = pe;
				break;
			}
	}
	if (entry)
		return (entry->create) (data);
	else
		return new UNKNOWN ();
}

}