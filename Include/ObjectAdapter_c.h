// ObjectAdapter Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTADAPTER_C_H_
#define NIRVANA_ORB_OBJECTADAPTER_C_H_

#include "ServantLinks_c.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapter;
typedef T_ptr <ObjectAdapter> ObjectAdapter_ptr;
typedef T_var <ObjectAdapter> ObjectAdapter_var;
typedef T_out <ObjectAdapter> ObjectAdapter_out;

template <>
class Bridge <ObjectAdapter> :
	public BridgeMarshal <ObjectAdapter>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
		}
		base;

		struct
		{
			BridgeMarshal <ServantLinks>* (*create_servant) (Bridge <ObjectAdapter>*, BridgeMarshal <ServantBase>*, const Char*, EnvironmentBridge*);
			BridgeMarshal <Object>* (*create_local_object) (Bridge <ObjectAdapter>*, BridgeMarshal <AbstractBase>*, const Char*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <ObjectAdapter> (epv.interface)
	{}
};

template <class T>
class Client <T, ObjectAdapter> :
	public T
{
public:
	ServantLinks_ptr create_servant (ServantBase_ptr servant, const Char* type_id);
	Object_ptr create_local_object (AbstractBase_ptr base, const Char* type_id);
};

class ObjectAdapter :
	public ClientInterfacePrimary <ObjectAdapter>,
	public ClientInterfaceBase <ObjectAdapter, AbstractBase>
{};

template <class T>
ServantLinks_ptr Client <T, ObjectAdapter>::create_servant (ServantBase_ptr servant, const Char* type_id)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b = (*this);
	ServantLinks_var _ret = (_b._epv ().epv.create_servant) (&_b, servant, type_id, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Object_ptr Client <T, ObjectAdapter>::create_local_object (AbstractBase_ptr base, const Char* type_id)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b = (*this);
	Object_var _ret = (_b._epv ().epv.create_local_object) (&_b, base, type_id, &_env);
	_env.check ();
	return _ret._retn ();
}

extern ObjectAdapter_ptr g_object_adapter;

}
}

#endif
