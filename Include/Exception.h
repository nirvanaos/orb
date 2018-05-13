// Nirvana project
// Object Request Broker
// Exceptions
#ifndef NIRVANA_ORB_EXCEPTION_H_
#define NIRVANA_ORB_EXCEPTION_H_

#include "BasicTypes.h"

namespace CORBA {

class Exception
{
public:
	virtual ~Exception ()
	{}
	virtual void _raise () const = 0;
	virtual const char* _name () const = 0;
	virtual const char* _rep_id () const = 0;

	// Nirvana specific
	virtual Long __code () const = 0;
	
	const void* __data () const
	{
		return this + 1;
	}

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
		return m_data.minor;
	}

	void minor (ULong minor)
	{
		m_data.minor = minor;
	}

	CompletionStatus completed () const
	{
		return m_data.completed;
	}

	void completed (CompletionStatus status)
	{
		m_data.completed = status;
	}

	virtual void _raise () const = 0;

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
		m_data.minor = 0;
		m_data.completed = COMPLETED_NO;
	}

	SystemException (const SystemException& src) :
		m_data (src.m_data)
	{}

	SystemException (ULong minor, CompletionStatus status = COMPLETED_NO)
	{
		m_data.minor = minor;
		m_data.completed = status;
	}

	SystemException (const Data* data)
	{
		if (data)
			m_data = *data;
		else {
			m_data.minor = 0;
			m_data.completed = COMPLETED_NO;
		}
	}

	SystemException& operator = (const SystemException& src)
	{
		m_data = src.m_data;
		return *this;
	}

	enum
	{  // System exception codes
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

		KNOWN_SYSTEM_EXCEPTIONS
	};

private:
	Data m_data;

	static const Nirvana::ExceptionEntry sm_create_table [KNOWN_SYSTEM_EXCEPTIONS];
};

#define DECLARE_SYSTEM_EXCEPTION(e) class e : public SystemException {\
public: e () {}\
e (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
e (const Data* data) : SystemException (data) {}\
virtual void _raise () const;\
virtual const char* _name () const;\
virtual const char* _rep_id () const;\
virtual Long __code () const;\
static const e* _downcast (const Exception* ep);\
static e* _downcast (Exception* ep) { return const_cast <e*> (_downcast ((const Exception*)ep)); }\
static const e* _narrow (const Exception* ep) { return _downcast (ep); }\
static e* _narrow (Exception* ep) { return _downcast (ep); }\
static Exception* _create (const void* data);\
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

#undef DECLARE_SYSTEM_EXCEPTION

#define OMGVMCID 0x4f4d0000
#define MAKE_MINOR(vmcid, c) (vmcid | c)
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (OMGVMCID, c))

class UserException : public Exception
{
	virtual Long __code () const
	{
		return -1;
	}
public:

	virtual void _raise () const = 0;

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
	UserException ();
};

} // namespace CORBA

#endif
