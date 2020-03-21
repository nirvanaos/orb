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
	ProxyObject (PortableServer::Servant servant, Interface_ptr lifecycle) :
		ProxyBase (servant, lifecycle),
		implicit_activation_ (false)
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

	virtual void add_ref_1 ();

	void implicit_deactivate ();

private:
	bool change_state (ActivationState from, ActivationState to)
	{
		return activation_state_.compare_exchange_strong (from, to);
	}

private:
	std::atomic <ActivationState> activation_state_;
	String implicit_activated_id_;
	bool implicit_activation_;
};

}
}
}

#endif
