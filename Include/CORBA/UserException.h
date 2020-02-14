#ifndef NIRVANA_ORB_USEREXCEPTION_H_
#define NIRVANA_ORB_USEREXCEPTION_H_

#include "Exception.h"

namespace CORBA {

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

}

#define DEFINE_USER_EXCEPTION(e, rep_id) \
::CORBA::Nirvana::ExceptionEntry _tc_##e { STATIC_BRIDGE (::CORBA::Nirvana::TypeCodeException <e>, ::CORBA::TypeCode) };\
::CORBA::TypeCode_ptr e::__type_code () const { return _tc_##e; } \
const e* e::_downcast (const ::CORBA::Exception* ep) { return (ep && ::CORBA::Nirvana::RepositoryId::compatible (ep->_rep_id (), rep_id)) ? static_cast <const e*> (ep) : nullptr; } \
DEFINE_EXCEPTION(e, rep_id)

#define DEFINE_INTERFACE_EXCEPTION(I, e, rep_id) \
::CORBA::Nirvana::ExceptionEntry I::_tc_##e { STATIC_BRIDGE (::CORBA::Nirvana::TypeCodeException <I::e>, ::CORBA::TypeCode) };\
::CORBA::TypeCode_ptr I::e::__type_code () const { return _tc_##e; } \
const I::e* I::e::_downcast (const ::CORBA::Exception* ep) { return (ep && ::CORBA::Nirvana::RepositoryId::compatible (ep->_rep_id (), rep_id)) ? static_cast <const I::e*> (ep) : nullptr; } \
DEFINE_EXCEPTION(I::e, rep_id)

#endif
