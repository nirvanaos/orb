#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

#include "Object.h"
#include "ReferenceCounter.h"
#include "String.h"
#include "UserException.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::I_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::I_var <POA> POA_var;
typedef ::CORBA::Nirvana::I_out <POA> POA_out;

}

namespace CORBA {
namespace Nirvana {

template <> class I_ptr <PortableServer::ServantBase>;

BRIDGE_BEGIN (PortableServer::POA, PORTABLESERVER_REPOSITORY_ID ("POA"))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Type <String>::ABI_ret (*activate_object) (Bridge <PortableServer::POA>*, Interface*, EnvironmentBridge*);
void (*deactivate_object) (Bridge <PortableServer::POA>*, Type <String>::ABI_in, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, PortableServer::POA> :
	public T
{
public:
	String activate_object (I_in <PortableServer::ServantBase> servant);
	void deactivate_object (String_in oid);
};

}
}

namespace PortableServer {

typedef ::CORBA::Nirvana::I_ptr <ServantBase> Servant;

class POA : public CORBA::Nirvana::ClientInterface <POA, CORBA::Object>
{
public:
	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ServantAlreadyActive;

	class ServantAlreadyActive : public ::CORBA::UserException
	{
	public:
		DECLARE_EXCEPTION (ServantAlreadyActive);
	};

	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ObjectNotActive;

	class ObjectNotActive : public ::CORBA::UserException
	{
	public:
		DECLARE_EXCEPTION (ObjectNotActive);
	};
};

}

#endif
