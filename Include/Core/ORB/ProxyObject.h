#ifndef NIRVANA_ORB_CORE_PROXYOBJECT_H_
#define NIRVANA_ORB_CORE_PROXYOBJECT_H_

#include "ServantProxyBase.h"
#include <atomic>

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyObject :
	public ServantProxyBase
{
	typedef ServantProxyBase Base;
	class Deactivator;

protected:
	ProxyObject (PortableServer::Servant servant) :
		ServantProxyBase (servant, object_ops_, this),
		servant_ (servant),
		implicit_activation_ (false)
	{}

private:
	enum ActivationState
	{
		INACTIVE,
		ACTIVATION,
		ACTIVE,
		DEACTIVATION_SCHEDULED,
		DEACTIVATION_CANCELLED
	};

	virtual void add_ref_1 ();
	virtual ::Nirvana::Core::AtomicCounter::UIntType _remove_ref ();

	void implicit_deactivate ();

	bool change_state (ActivationState from, ActivationState to)
	{
		return activation_state_.compare_exchange_strong (from, to);
	}

	static void get_interface_request (ProxyObject* servant,
		IORequest_ptr call,
		::Nirvana::ConstPointer in_params,
		Unmarshal_var unmarshaler,
		::Nirvana::Pointer out_params);

	static void is_a_request (ProxyObject* servant,
		IORequest_ptr call,
		::Nirvana::ConstPointer in_params,
		Unmarshal_var unmarshaler,
		::Nirvana::Pointer out_params);

	static void non_existent_request (ProxyObject* servant,
		IORequest_ptr call,
		::Nirvana::ConstPointer in_params,
		Unmarshal_var unmarshaler,
		::Nirvana::Pointer out_params);

private:
	PortableServer::Servant servant_;
	std::atomic <ActivationState> activation_state_;
	String implicit_activated_id_;
	bool implicit_activation_;

	static const Operation object_ops_ [3];
};

}
}
}

#endif
