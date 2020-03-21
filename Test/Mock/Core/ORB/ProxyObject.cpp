#include "ProxyObject.h"
#include "../Runnable.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyObject::Deactivator :
	public ::Nirvana::Core::Runnable <Deactivator>
{
public:
	Deactivator (ProxyObject& proxy) :
		proxy_ (proxy)
	{}

	~Deactivator ()
	{}

	void run ()
	{
		proxy_.implicit_deactivate ();
	}

private:
	ProxyObject& proxy_;
};

// Called in the servant synchronization context.
// Note that sync context may be out of synchronization domain
// for the stateless objects.
void ProxyObject::add_ref_1 ()
{
	Base::add_ref_1 ();
	if (
		!change_state (DEACTIVATION_SCHEDULED, DEACTIVATION_CANCELLED)
	&&
		change_state (INACTIVE, ACTIVATION)
	) {
		try {
			PortableServer::POA_var poa = servant_->_default_POA ();
			// TODO: Query poa for the implicit activation policy
			// While assume that implicit activation is on
			implicit_activation_ = true;
			implicit_activated_id_ = poa->activate_object (servant_);
			activation_state_ = ACTIVE;
		} catch (...) {
			activation_state_ = INACTIVE;
			throw;
		}
	}
}

::Nirvana::Core::AtomicCounter::UIntType ProxyObject::_remove_ref ()
{
	::Nirvana::Core::AtomicCounter::UIntType cnt = Base::_remove_ref ();
	if (implicit_activation_ && 1 == cnt) {
		// Launch deactivator
		if (
			!change_state (DEACTIVATION_CANCELLED, DEACTIVATION_SCHEDULED)
		&&
			change_state (ACTIVE, DEACTIVATION_SCHEDULED)
		) {
			try {
				run_garbage_collector ((new Deactivator (*this))->_get_ptr ());
			} catch (...) {
				// Async call failed, maybe resources are exausted.
				// Fallback to collect garbage in current thread.
				// TODO: Log the error
				try {
					::Nirvana::Synchronized sync (sync_context ());
					implicit_deactivate ();
				} catch (...) {
					// TODO: Log the error
				}
			}
		}
	}
	return cnt;
}

// Called from Deactivator in the servant synchronization context.
void ProxyObject::implicit_deactivate ()
{
	String tmp = std::move (implicit_activated_id_);
	if (change_state (DEACTIVATION_SCHEDULED, INACTIVE)) {
		servant_->_default_POA ()->deactivate_object (tmp);
	} else {
		implicit_activated_id_ = std::move (tmp);
		assert (DEACTIVATION_CANCELLED == activation_state_);
		if (!change_state (DEACTIVATION_CANCELLED, ACTIVE))
			::Nirvana::throw_BAD_INV_ORDER ();
	}
}

}
}
}
