// Nirvana project.
// Object server stub implementation.
#ifndef NIRVANA_TESTORB_OBJECTCORE_H_
#define NIRVANA_TESTORB_OBJECTCORE_H_

#include <Object_s.h>
#include <Implementation.h>

namespace CORBA {
namespace Nirvana {

class ObjectCore :
	public ServantTraits <ObjectCore>,
	public InterfaceImplBase <ObjectCore, Object>
{
public:
	ObjectCore (ServantBase_ptr servant) :
		servant_ (servant)
	{}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env);

	template <>
	static Bridge <AbstractBase>* _wide <AbstractBase, Object> (Bridge <Object>* derived, const Char* id, EnvironmentBridge* env)
	{
		ServantBase_ptr servant = _implementation (derived).servant_;
		return (servant->_epv ().base.CORBA_AbstractBase) (servant, id, env);
	}

	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		Interface_ptr servant = _implementation (static_cast <Bridge <I>*> (itf)).servant_;
		return (servant->_epv ().duplicate) (servant, env);
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		Interface_ptr servant = _implementation (static_cast <Bridge <I>*> (itf)).servant_;
		return (servant->_epv ().release) (servant);
	}

	static BridgeMarshal <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return nullptr;
	}

	static BridgeMarshal <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		ServantBase_ptr servant = _implementation (obj).servant_;
		return (servant->_epv ().epv.get_interface) (servant, env);
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		ServantBase_ptr servant = _implementation (obj).servant_;
		return (servant->_epv ().epv.is_a) (servant, type_id, env);
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		ServantBase_ptr servant = _implementation (obj).servant_;
		return (servant->_epv ().epv.non_existent) (servant, env);
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, BridgeMarshal <Object>* other, EnvironmentBridge*)
	{
		return obj == other;
	}

	ULong _hash (ULong maximum)
	{
		return 0; // TODO: Implement.
	}
	// TODO: Other Object operations shall be here...

private:
	ServantBase_ptr servant_;
};

}
}

#endif
