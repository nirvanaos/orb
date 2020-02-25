// Nirvana project.
// Object server stub implementation.
#ifndef NIRVANA_TESTORB_OBJECTCORE_H_
#define NIRVANA_TESTORB_OBJECTCORE_H_

#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

class ObjectCore :
	public ObjectImpl <ObjectCore>,
	public LifeCycleRefCnt <ObjectCore>
{
public:
	ObjectCore (ServantBase_ptr servant) :
		ObjectImpl <ObjectCore> (servant),
		is_active_ (false),
		servant_ (servant)
	{}

	bool is_active_;

	// Delegate to base
	void _add_ref ()
	{
		ServantBase::_duplicate (servant_);
	}

	void _remove_ref ()
	{
		release (servant_);
	}

	// Object operations delegated to ServantBase.

	static Interface* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.get_interface) (servant, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static ABI_boolean __is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.is_a) (servant, type_id, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static ABI_boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			ServantBase_ptr servant = _implementation (obj).servant_;
			return (servant->_epv ().epv.non_existent) (servant, env);
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

private:
	ServantBase_ptr servant_;
};

}
}

#endif
