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
	public LifeCycleDynamic <ObjectCore>,
	public InterfaceImplBase <ObjectCore, Object>
{
public:
	ObjectCore (ServantBase_ptr servant, DynamicServant_ptr dynamic) :
		servant_ (servant), dynamic_ (dynamic),
		is_active_ (false)
	{}

	bool is_active_;

	AbstractBase_ptr abstract_base () const
	{
		return dynamic_;
	}

	const Char* primary_interface () const
	{
		return dynamic_->_primary_interface ();
	}

	// Delegate to base
	
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env);

	template <>
	static Bridge <AbstractBase>* _wide <AbstractBase, Object> (Bridge <Object>* derived, const Char* id, EnvironmentBridge* env)
	{
		try {
			DynamicServant_ptr servant = _implementation (derived).dynamic_;
			return (servant->_epv ().base.CORBA_AbstractBase) (servant, id, env);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env);

	template <>
	static Bridge <Interface>* __duplicate <Object> (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			ObjectCore& _this = _implementation (static_cast <Bridge <Object>*> (itf));
			Interface_ptr servant = _this.dynamic_;
			(servant->_epv ().duplicate) (servant, env);
			return &_this;
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static void _release (Bridge <Object>* itf)
	{
		release (_implementation (itf).dynamic_);
	}

	// Object operations

	static BridgeMarshal <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return nullptr;
	}

	static BridgeMarshal <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.get_interface) (servant, env);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.is_a) (servant, type_id, env);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.non_existent) (servant, env);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, BridgeMarshal <Object>* other, EnvironmentBridge*)
	{
		return obj == other;
	}

	ULong _hash (ULong maximum) const
	{
		return 0; // TODO: Implement.
	}
	// TODO: Other Object operations shall be here...

private:
	ServantBase_ptr servant_;
	DynamicServant_ptr dynamic_;
};

}
}

#endif
