// Nirvana project.
// Object server stub implementation.
#ifndef NIRVANA_TESTORB_OBJECTCORE_H_
#define NIRVANA_TESTORB_OBJECTCORE_H_

#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

class ObjectCore :
	public ServantTraits <ObjectCore>,
	public LifeCycleDynamic <ObjectCore>,
	public ObjectImpl <ObjectCore>
{
public:
	ObjectCore (PortableServer::Servant servant, DynamicServant_ptr dynamic) :
		ObjectImpl <ObjectCore> (dynamic),
		servant_ (servant),
		is_active_ (false)
	{}

	bool is_active_;

	// Delegate to base
	
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env);

	template <>
	static Bridge <AbstractBase>* _wide <AbstractBase, Object> (Bridge <Object>* derived, const Char* id, EnvironmentBridge* env)
	{
		try {
			DynamicServant_ptr servant = _implementation (derived).servant ();
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
			Interface_ptr servant = _this.servant ();
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
		release (_implementation (itf).servant ());
	}

	// Object operations delegated to ServantBase.

	static BridgeMarshal <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			PortableServer::Servant servant = _implementation (obj).servant_;
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
			PortableServer::Servant servant = _implementation (obj).servant_;
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
			PortableServer::Servant servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.non_existent) (servant, env);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

private:
	PortableServer::Servant servant_;
};

}
}

#endif
