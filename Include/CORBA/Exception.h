// Nirvana project
// Object Request Broker
// Exceptions
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include "T_ptr.h"

#define OMGVMCID 0x4f4d0000
#define MAKE_MINOR(vmcid, c) (vmcid | c)
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (OMGVMCID, c))

#define DECLARE_EXCEPTION(e) \
virtual void raise () const;\
virtual const char* _rep_id () const;\
static const char repository_id_ [];\
virtual const char* _name () const;\
static constexpr const char* __name () { return #e; }\
virtual TypeCode_ptr __type_code () const;\
virtual Exception* __clone () const;\
static const e* _downcast (const Exception* ep);\
static e* _downcast (Exception* ep) { return const_cast <e*> (_downcast ((const Exception*)ep)); }\
static const e* _narrow (const Exception* ep) { return _downcast (ep); }\
static e* _narrow (Exception* ep) { return _downcast (ep); }\
static Exception* _create (const void* data);

#define DECLARE_SYSTEM_EXCEPTION(e) \
extern const Nirvana::StaticInterface <TypeCode> _tc_##e;\
class e : public SystemException {\
public: e () {}\
e (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
e (const Data* data) : SystemException (data) {}\
virtual Long __code () const;\
DECLARE_EXCEPTION(e)\
};

#define DEFINE_EXCEPTION(e, rep_id) \
void e::raise () const { throw *this; } \
const char e::repository_id_ [] = rep_id; \
const char* e::_rep_id () const { return repository_id_; } \
const char* e::_name () const { return __name (); } \
Exception* e::__clone () const { return new e (*this); } \
Exception* e::_create (const void* data) { return new e ((Data*)data); } // TODO : Remove

#define DEFINE_INTERFACE_EXCEPTION(I, e, rep_id) \
const Nirvana::StaticInterface <TypeCode> I::_tc_##e { STATIC_BRIDGE (Nirvana::TypeCodeException <I::e>, TypeCode) };\
TypeCode_ptr I::e::__type_code () const { return _tc_##e; } \
const I::e* I::e::_downcast (const Exception* ep) { return (ep && ::CORBA::Nirvana::RepositoryId::compatible (ep->_rep_id (), rep_id)) ? static_cast <const I::e*> (ep) : nullptr; } \
DEFINE_EXCEPTION(I::e, rep_id)

#define EX_TABLE_ENTRY(e) { e::repository_id_, e::_create }

namespace CORBA {

class TypeCode;
typedef Nirvana::T_ptr <TypeCode> TypeCode_ptr;

class Exception
{
public:
	typedef void Data;

	virtual ~Exception ()
	{}
	virtual void raise () const = 0;
	virtual const char* _name () const = 0;
	virtual const char* _rep_id () const = 0;

	// Nirvana specific
	virtual Long __code () const = 0;
	virtual TypeCode_ptr __type_code () const = 0;
	virtual Exception* __clone () const = 0;	// TODO: Remove!

	const void* __data () const
	{
		return this + 1;
	}

	enum {
		EC_NO_EXCEPTION = -2,
		EC_USER_EXCEPTION = -1
	};

protected:
	Exception ()
	{}

	Exception (const Exception&)
	{}

	Exception& operator = (const Exception&)
	{
		return *this;
	}
};

namespace Nirvana {

typedef ::CORBA::Exception* (*ExceptionCreateProc) (const void* data);

struct ExceptionEntry
{
	const char* rep_id;
	ExceptionCreateProc create;
};

}

enum CompletionStatus
{
	COMPLETED_YES,
	COMPLETED_NO,
	COMPLETED_MAYBE
};

class SystemException : public Exception
{
public:
	struct Data
	{
		ULong minor;
		CompletionStatus completed;
	};

	ULong minor () const
	{
		return data_.minor;
	}

	void minor (ULong minor)
	{
		data_.minor = minor;
	}

	CompletionStatus completed () const
	{
		return data_.completed;
	}

	void completed (CompletionStatus status)
	{
		data_.completed = status;
	}

	static const SystemException* _downcast (const Exception* ep)
	{
		return (ep && (ep->__code () >= 0)) ? static_cast <const SystemException*> (ep) : 0;
	}

	static SystemException* _downcast (Exception* ep)
	{
		return const_cast <SystemException*> (_downcast ((const Exception*)ep));
	}

	static const SystemException* _narrow (const Exception* ep)
	{
		return _downcast (ep);
	}

	static SystemException* _narrow (Exception* ep)
	{
		return _downcast (ep);
	}

	static Exception* _create (const char* rep_id, const void* data, int hint = -1);

protected:
	SystemException ()
	{
		data_.minor = 0;
		data_.completed = COMPLETED_NO;
	}

	SystemException (const SystemException& src) :
		data_ (src.data_)
	{}

	SystemException (ULong minor, CompletionStatus status = COMPLETED_NO)
	{
		data_.minor = minor;
		data_.completed = status;
	}

	SystemException (const Data* data)
	{
		if (data)
			data_ = *data;
		else {
			data_.minor = 0;
			data_.completed = COMPLETED_NO;
		}
	}

	SystemException& operator = (const SystemException& src)
	{
		data_ = src.data_;
		return *this;
	}

	enum
	{
		// System exception codes
		EC_UNKNOWN = 0,
		EC_BAD_PARAM,
		EC_NO_MEMORY,
		EC_IMP_LIMIT,
		EC_COMM_FAILURE,
		EC_INV_OBJREF,
		EC_NO_PERMISSION,
		EC_INTERNAL,
		EC_MARSHAL,
		EC_INITIALIZE,
		EC_NO_IMPLEMENT,
		EC_BAD_TYPECODE,
		EC_BAD_OPERATION,
		EC_NO_RESOURCES,
		EC_NO_RESPONSE,
		EC_PERSIST_STORE,
		EC_BAD_INV_ORDER,
		EC_TRANSIENT,
		EC_FREE_MEM,
		EC_INV_IDENT,
		EC_INV_FLAG,
		EC_INTF_REPOS,
		EC_BAD_CONTEXT,
		EC_OBJ_ADAPTER,
		EC_DATA_CONVERSION,
		EC_OBJECT_NOT_EXIST,
		EC_TRANSACTION_REQUIRED,
		EC_TRANSACTION_ROLLEDBACK,
		EC_INVALID_TRANSACTION,
		EC_INV_POLICY,
		EC_CODESET_INCOMPATIBLE,
		EC_REBIND,
		EC_TIMEOUT,
		EC_TRANSACTION_UNAVAILABLE,
		EC_TRANSACTION_MODE,
		EC_BAD_QOS,

		// Nirvana-specific
		EC_MEM_NOT_COMMITTED,
		EC_MEM_NOT_ALLOCATED,

		KNOWN_SYSTEM_EXCEPTIONS
	};

private:
	Data data_;

	static const Nirvana::ExceptionEntry creators_ [KNOWN_SYSTEM_EXCEPTIONS];
};

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

#undef DECLARE_SYSTEM_EXCEPTION

class UserException : public Exception
{
	virtual Long __code () const
	{
		return EC_USER_EXCEPTION;
	}
public:
	static const UserException* _downcast (const Exception* ep)
	{
		return (ep && (ep->__code () < 0)) ? static_cast <const UserException*> (ep) : 0;
	}

	static UserException* _downcast (Exception* ep)
	{
		return const_cast <UserException*> (_downcast ((const Exception*)ep));
	}

	static const UserException* _narrow (const Exception* ep)
	{
		return _downcast (ep);
	}

	static UserException* _narrow (Exception* ep)
	{
		return _downcast (ep);
	}

protected:
	UserException ()
	{}
};

} // namespace CORBA

#endif
