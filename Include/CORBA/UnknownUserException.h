#ifndef NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_
#define NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_

#include "UserException.h"
#include "Any.h"

namespace CORBA {

class UnknownUserException : public UserException
{
public:
	DECLARE_EXCEPTION (UnknownUserException);

	typedef Any Data;

	const Any& exception () const
	{
		return data_;
	}

private:
	Data data_;
};

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_UnknownUserException;

}

#endif
