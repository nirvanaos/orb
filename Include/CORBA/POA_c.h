#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

#include "Object.h"
#include "ReferenceCounter.h"
#include "String.h"

namespace PortableServer {

class ServantBase;
template <> class ::CORBA::Nirvana::I_ptr <ServantBase>;
typedef ::CORBA::Nirvana::I_ptr <ServantBase> ServantBase_ptr;
typedef ::CORBA::Nirvana::I_var <ServantBase> ServantBase_var;
typedef ::CORBA::Nirvana::I_out <ServantBase> ServantBase_out;

class POA;
typedef ::CORBA::Nirvana::I_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::I_var <POA> POA_var;
typedef ::CORBA::Nirvana::I_out <POA> POA_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (PortableServer::POA, PORTABLESERVER_REPOSITORY_ID (POA))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Type <String>::ABI_ret (*activate_object) (Bridge <PortableServer::POA>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, PortableServer::POA> :
	public T
{
public:
	String activate_object (I_in <PortableServer::ServantBase> servant);
};

}
}

namespace PortableServer {

class POA : public CORBA::Nirvana::ClientInterface <POA, CORBA::Object>
{};

}

#endif
