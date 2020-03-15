#ifndef NIRVANA_ORB_USEREXCEPTION_H_
#define NIRVANA_ORB_USEREXCEPTION_H_

#include "Exception.h"

namespace CORBA {

class UserException : public Exception
{
	virtual Long __code () const NIRVANA_NOEXCEPT
	{
		return EC_USER_EXCEPTION;
	}

public:
	static const UserException* _downcast (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return (ep && (ep->__code () < 0)) ? static_cast <const UserException*> (ep) : 0;
	}

	static UserException* _downcast (Exception* ep) NIRVANA_NOEXCEPT
	{
		return const_cast <UserException*> (_downcast ((const Exception*)ep));
	}

	static const UserException* _narrow (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

	static UserException* _narrow (Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

protected:
	UserException ()
	{}
};

}

#define DEFINE_USER_EXCEPTION(E)const E* E::_downcast (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT {\
return (ep && ::CORBA::Nirvana::RepositoryId::compatible (ep->_rep_id (), repository_id_)) ? &static_cast <const E&> (*ep) : 0; }

#define DEFINE_USER_EXCEPTION1(prefix, ns, E, major, minor) DEFINE_EXCEPTION1 (prefix, ns, E, major, minor) DEFINE_USER_EXCEPTION (E)
#define DEFINE_USER_EXCEPTION2(prefix, ns1, ns2, E, major, minor) DEFINE_EXCEPTION2 (prefix, ns1, ns2, E, major, minor) DEFINE_USER_EXCEPTION (E)

#define DEFINE_CORBA_INTERFACE_EXCEPTION(ns, I, E)\
const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> I::_tc_##E { ::Nirvana::OLF_IMPORT_INTERFACE, #ns "/" #I "/_tc_" #E, ::CORBA::TypeCode::interface_id_ };\
::CORBA::TypeCode_ptr I::E::__type_code () const NIRVANA_NOEXCEPT { return _tc_##E; }\
const char I::E::repository_id_ [] = "IDL:omg.org/" #ns "/" #E ":1.0";\
DEFINE_USER_EXCEPTION(I::E)

#endif
