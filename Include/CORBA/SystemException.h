#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_H_

#include "Exception.h"

#define OMGVMCID 0x4f4d0000
#define MAKE_MINOR(vmcid, c) (vmcid | c)
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (OMGVMCID, c))

namespace CORBA {

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

private:
	Data data_;

	static const Nirvana::ExceptionEntry creators_[KNOWN_SYSTEM_EXCEPTIONS];
};

}

#endif
