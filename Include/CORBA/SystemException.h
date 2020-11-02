#ifndef NIRVANA_ORB_SYSTEMEXCEPTION_H_
#define NIRVANA_ORB_SYSTEMEXCEPTION_H_

#include "Exception.h"
#include "system_exceptions.h"
#include "TypeEnum.h"
#include <Nirvana/ImportInterface.h>

#define OMGVMCID 0x4f4d0000
#define MAKE_MINOR(vmcid, c) (vmcid | c)
#define MAKE_OMG_MINOR(c) (MAKE_MINOR (OMGVMCID, c))

namespace CORBA {

enum CompletionStatus : uint32_t
{
	COMPLETED_YES,
	COMPLETED_NO,
	COMPLETED_MAYBE
};

class NIRVANA_NOVTABLE SystemException : public Exception
{
public:
	struct Data
	{
		uint32_t minor;
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

	static const Nirvana::ExceptionEntry* _get_exception_entry (String_in rep_id, Code hint = Exception::EC_USER_EXCEPTION);
	static const Nirvana::ExceptionEntry* _get_exception_entry (TypeCode_ptr tc);

	static void _raise_code (Code ec);

#define EXCEPTION_CODE(e) EC_##e,

	enum : Code
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

	SystemException& operator = (const SystemException& src)
	{
		data_ = src.data_;
		return *this;
	}

private:
	Data data_;

	struct ExceptionEntry
	{
		Nirvana::ExceptionEntry ee;
		size_t rep_id_len;
	};

	static const ExceptionEntry exception_entries_ [KNOWN_SYSTEM_EXCEPTIONS];
};

namespace Nirvana {

template <>
struct Type <CompletionStatus> :
	public TypeEnum <CompletionStatus, COMPLETED_MAYBE>
{};

template <>
const Char RepIdOf <CompletionStatus>::repository_id_ [] = CORBA_REPOSITORY_ID ("CompletionStatus");

}

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_CompletionStatus;

}

#endif
