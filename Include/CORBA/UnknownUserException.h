#ifndef NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_
#define NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_

#include "UserException.h"
#include "Any.h"

namespace CORBA {

class UnknownUserException : public UserException
{
	virtual Long __code () const NIRVANA_NOEXCEPT
	{
		return EC_UNKNOWN_USER_EXCEPTION;
	}

public:
	static const UnknownUserException* _downcast (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return (ep && (ep->__code () == EC_UNKNOWN_USER_EXCEPTION)) ? static_cast <const UnknownUserException*> (ep) : 0;
	}

	static UnknownUserException* _downcast (Exception* ep) NIRVANA_NOEXCEPT
	{
		return const_cast <UnknownUserException*> (_downcast ((const Exception*)ep));
	}

	static const UnknownUserException* _narrow (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

	static UnknownUserException* _narrow (Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

	const Any& exception () const
	{
		return exception_;
	}

	virtual void _raise () const;
	virtual const char* _name () const NIRVANA_NOEXCEPT;
	virtual const char* _rep_id () const NIRVANA_NOEXCEPT;
	virtual TypeCode_ptr __type_code () const NIRVANA_NOEXCEPT;

private:
	Any exception_;
};

}

#endif
