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

	typedef Any _Data;

	const Any& exception () const
	{
		return _data;
	}

	UnknownUserException (Any&& exc) :
		_data (std::move (exc))
	{}

private:
	_Data _data;
};

}

#endif
