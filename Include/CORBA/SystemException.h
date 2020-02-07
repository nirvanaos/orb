#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_H_

#include "Exception.h"
#include "system_exceptions.h"

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

	static TypeCode_ptr _get_type_code (const char* rep_id, int hint = EC_SYSTEM_EXCEPTION);

#define EXCEPTION_CODE(e) EC_##e,

	enum
	{
		// System exception codes
		SYSTEM_EXCEPTIONS (EXCEPTION_CODE)

		KNOWN_SYSTEM_EXCEPTIONS
	};

#undef EXCEPTION_CODE

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

	static const Nirvana::ExceptionEntry type_codes_ [KNOWN_SYSTEM_EXCEPTIONS];
};

}

#endif
