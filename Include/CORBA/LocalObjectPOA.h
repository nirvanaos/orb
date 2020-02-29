#ifndef NIRVANA_ORB_LOCALOBJECTPOA_H_
#define NIRVANA_ORB_LOCALOBJECTPOA_H_

#include "ServantBasePOA.h"
#include "ObjectLink.h"
#include "Object_s.h"
#include "LocalObject_s.h"

namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA <Object> :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public InterfaceImplBase <ServantPOA <Object>, Object>,
	public ObjectLink
{
public:
	// Static overrides to resolve the ambiguity with ServantBase.
	static Interface* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env);
	static ABI_boolean __is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env);
	static ABI_boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env);

	// Delegate ReferenceCounter to AbstractBase
	virtual void _add_ref ()
	{
		ServantPOA <AbstractBase>::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		ServantPOA <AbstractBase>::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return ServantPOA <AbstractBase>::_refcount_value ();
	}

	// Object operations

	virtual ImplementationDef_ptr _get_implementation ()
	{
		return ObjectLink::_get_implementation ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ObjectLink::_get_interface ();
	}

	virtual Boolean _is_a (const String& type_id)
	{
		return ObjectLink::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return ObjectLink::_non_existent ();
	}

	virtual Boolean _is_equivalent (Object_ptr other_object)
	{
		return ObjectLink::_is_equivalent (other_object);
	}

	virtual ULong _hash (ULong maximum)
	{
		return ObjectLink::_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	ServantPOA ();

	virtual Interface* _get_proxy ();
};

template <>
class ServantPOA <LocalObject> :
	public ServantPOA <Object>,
	public InterfaceImplBase <ServantPOA <LocalObject>, LocalObject>
{};

}
}

#endif
