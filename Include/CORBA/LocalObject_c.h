#ifndef NIRVANA_ORB_LOCALOBJECT_C_H_
#define NIRVANA_ORB_LOCALOBJECT_C_H_

#include "Object_c.h"
#include "ReferenceCounter_c.h"

namespace CORBA {

class LocalObject;
typedef Nirvana::T_ptr <LocalObject> LocalObject_ptr;
typedef Nirvana::T_var <LocalObject> LocalObject_var;
typedef Nirvana::T_out <LocalObject> LocalObject_out;
typedef Nirvana::T_inout <LocalObject> LocalObject_inout;

namespace Nirvana {

template <>
class Bridge <LocalObject> :
	public BridgeMarshal <LocalObject>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
			BASE_STRUCT_ENTRY (ReferenceCounter, _ReferenceCounter)
		}
		base;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <LocalObject> (epv.interface)
	{}
};

}

class LocalObject : public Nirvana::ClientInterface <LocalObject, Object, Nirvana::ReferenceCounter>
{};

}

#endif
