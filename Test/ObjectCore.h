// Nirvana project.
// Object server stub implementation.
#ifndef NIRVANA_TESTORB_OBJECTCORE_H_
#define NIRVANA_TESTORB_OBJECTCORE_H_

#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

class ObjectCore :
	public ServantTraits <ObjectCore>,
	public LifeCycleRefCnt <ObjectCore>,
	public ObjectImpl <ObjectCore>
{
public:
	ObjectCore (PortableServer::Servant servant) :
		ObjectImpl <ObjectCore> (servant),
		is_active_ (false),
		servant_ (servant),
		reference_counter_ (servant)
	{}

	bool is_active_;

	// Delegate to base
	void _add_ref ()
	{
		reference_counter_->_add_ref ();
	}

	void _remove_ref ()
	{
		reference_counter_->_remove_ref ();
	}

	// Object operations delegated to ServantBase.

	static BridgeMarshal <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			PortableServer::Servant servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.get_interface) (servant, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			PortableServer::Servant servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.is_a) (servant, type_id, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			PortableServer::Servant servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.non_existent) (servant, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

private:
	PortableServer::Servant servant_;
	ReferenceCounter_ptr reference_counter_;
};

}
}

#endif
