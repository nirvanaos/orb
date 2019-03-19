#ifndef NIRVANA_TESTORB_OBJECTIMPL_H_
#define NIRVANA_TESTORB_OBJECTIMPL_H_

#include <Implementation.h>

namespace CORBA {
namespace Nirvana {

class ObjectBase
{
public:
	ObjectBase (DynamicServant_ptr servant) :
		servant_ (servant)
	{}

	DynamicServant_ptr servant () const
	{
		return servant_;
	}

	// Object operations

	static BridgeMarshal <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return nullptr; // We dont implement it
	}

	InterfaceDef_ptr _get_interface () const
	{
		return InterfaceDef_ptr::nil ();	// TODO: Implement
	}

	Boolean _is_a (const Char* type_id) const;

	static Boolean __non_existent (Bridge <Object>*, EnvironmentBridge*)
	{
		return FALSE;
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, BridgeMarshal <Object>* other, EnvironmentBridge*)
	{
		return obj == other;
	}

	ULong _hash (ULong maximum) const
	{
		return 0; // TODO: Implement.
	}
	// TODO: More Object operations shall be here...
	
private:
	DynamicServant_ptr servant_;
};

template <class S>
class ObjectImpl :
	public InterfaceImplBase <S, Object>,
	public ObjectBase
{
public:
	ObjectImpl (DynamicServant_ptr servant) :
		ObjectBase (servant)
	{}

	static BridgeMarshal <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return ObjectBase::__get_implementation (obj, env);
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return ObjectBase::__non_existent (obj, env);
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, BridgeMarshal <Object>* other, EnvironmentBridge* env)
	{
		return ObjectBase::__is_equivalent (obj, other, env);
	}
};

}
}

#endif
