#include <CORBA/UnknownUserException.h>

namespace CORBA {

void UnknownUserException::_raise () const
{
	throw* this;
}

const char* UnknownUserException::_name () const NIRVANA_NOEXCEPT
{
	try {
		return exception_.type ()->name ();
	} catch (...) {
		return "";
	}
}

const char* UnknownUserException::_rep_id () const NIRVANA_NOEXCEPT
{
	try {
		return exception_.type ()->id ();
	} catch (...) {
		return "";
	}
}

TypeCode_ptr UnknownUserException::__type_code () const NIRVANA_NOEXCEPT
{
	try {
		return exception_.type ();
	} catch (...) {
		return TypeCode::_nil ();
	}
}

}
