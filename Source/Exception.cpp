#include "Exception.h"
#include "RepositoryId.h"

#define DEFINE_SYSTEM_EXCEPTION(e)\
void e::_raise () const { throw *this; }\
const char* e::_name () const { return #e; }\
const char* e::_rep_id () const { return CORBA_REPOSITORY_ID (e); }\
Long e::__code () const { return EC_##e; }\
const e* e::_downcast (const Exception* ep) { return (ep && (EC_##e == ep->__code ())) ? static_cast <const e*> (ep) : 0; }\
Exception* e::_create (const void* data) { return new e ((Data*)data); }

#define EX_TABLE_ENTRY(e) { CORBA_REPOSITORY_ID (e), e::_create }

namespace CORBA {

using namespace Nirvana;

const ExceptionEntry SystemException::sm_create_table [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
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
};

Exception* SystemException::_create (const char* rep_id, const void* data, int hint)
{
	const ExceptionEntry* entry = 0;
	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS && Nirvana::RepositoryId::compatible (sm_create_table [hint].rep_id, rep_id))
		entry = sm_create_table + hint;
	else {
		for (const ExceptionEntry* pe = sm_create_table; pe != sm_create_table + KNOWN_SYSTEM_EXCEPTIONS; ++pe)
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

}