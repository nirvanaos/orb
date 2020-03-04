#ifndef NIRVANA_ORB_CORE_PROXYOBJECT_H_
#define NIRVANA_ORB_CORE_PROXYOBJECT_H_

#include "ProxyBase.h"
#include "ObjectImpl.h"
#include <atomic>

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyObject :
	public ProxyBase <ProxyObject, PortableServer::ServantBase>,
	public ObjectImpl
{
	typedef ProxyBase <ProxyObject, PortableServer::ServantBase> Base;
	class Deactivator;
public:
	ProxyObject (PortableServer::Servant servant) :
		ProxyBase (servant),
		activation_cnt_ (0),
		deactivator_launched_ (0)
	{}

	Boolean _is_equivalent (Object_ptr other) const
	{
		return default_is_equivalent (other);
	}

	virtual ::Nirvana::Core::AtomicCounter::UIntType _remove_ref ();

private:
	enum ActivationState
	{
		INACTIVE,
		ACTIVATION,
		ACTIVE,
		DEACTIVATION_SCHEDULED,
		DEACTIVATION_CANCELLED
	};

	// Called in the servant synchronization context.
	// Note that sync context may be out of synchronization domain
	// for the stateless objects.
	virtual void add_ref_1 ();

	void implicit_deactivate ();

private:
	bool change_state (ActivationState from, ActivationState to)
	{
		return activation_state_.compare_exchange_strong (from, to);
	}

private:
	std::atomic <ActivationState> activation_state_;
	::Nirvana::Core::AtomicCounter activation_cnt_;
	::Nirvana::Core::AtomicCounter deactivator_launched_;
	String implicit_activated_id_;
	bool implicit_activation_;
};

}
}
}

#endif
