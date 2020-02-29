#ifndef NIRVANA_ORB_LOCALOBJECTPOA_H_
#define NIRVANA_ORB_LOCALOBJECTPOA_H_

#include "ServantBasePOA.h"
#include "ObjectLink.h"
#include "Object_s.h"
#include "LocalObject_s.h"

#ifdef _MSC_BUILD
#pragma warning (disable:4250)
#endif

namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA <Object> :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public Skeleton <ServantPOA <Object>, Object>,
	public ObjectLink
{
public:
	// Override operator from ServantBase
	virtual operator Bridge <Object>& ()
	{
		return *this;
	}

	// Static overrides to resolve the ambiguity with ServantBase.
	static Interface* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env);
	static ABI_boolean __is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env);
	static ABI_boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env);

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
	ServantPOA () :
		ObjectLink (Skeleton <ServantPOA <Object>, Object>::epv_)
	{}

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
