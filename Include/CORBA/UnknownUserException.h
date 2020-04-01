#ifndef NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_
#define NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_

#include "UserException.h"
#include "Any.h"

namespace CORBA {

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_UnknownUserException;

class UnknownUserException : public UserException
{
public:
	DECLARE_EXCEPTION (UnknownUserException);

	typedef Any Data;

	const Any& exception () const
	{
		return data_;
	}

	UnknownUserException (Any&& exc) :
		data_ (std::move (exc))
	{}

private:
	Data data_;
};

}

#endif
