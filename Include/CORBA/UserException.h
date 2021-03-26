#ifndef NIRVANA_ORB_USEREXCEPTION_H_
#define NIRVANA_ORB_USEREXCEPTION_H_

#include "Exception.h"

namespace CORBA {

class UserException : public Exception
{
	virtual Code __code () const NIRVANA_NOEXCEPT
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

#define DEFINE_USER_EXC(E)const E* E::_downcast (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT {\
return (ep && ::CORBA::Nirvana::RepositoryId::compatible (ep->_rep_id (), E::repository_id_)) ? &static_cast <const E&> (*ep) : 0; }\

#define DEFINE_USER_EXCEPTION(E, rep_id) const ::CORBA::Char E::repository_id_ [] = rep_id;\
NIRVANA_OLF_SECTION extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode>\
_tc_##E = { ::Nirvana::OLF_IMPORT_INTERFACE, E::repository_id_, ::CORBA::TypeCode::repository_id_ };\
DEFINE_USER_EXC (E)\
GNU_OPTNONE ::CORBA::TypeCode_ptr E::__type_code () const NIRVANA_NOEXCEPT { return _tc_##E; }

#define DEFINE_CORBA_INTERFACE_EXCEPTION(ns, I, E)\
const ::CORBA::Char CORBA::Nirvana::Definitions <ns::I>::E::repository_id_ [] = "IDL:omg.org/" #ns "/" #I "/" #E ":1.0";\
DEFINE_USER_EXC (CORBA::Nirvana::Definitions <ns::I>::E)\
GNU_OPTNONE ::CORBA::TypeCode_ptr CORBA::Nirvana::Definitions <ns::I>::E::__type_code () const NIRVANA_NOEXCEPT { return _tc_##E; }

#endif
