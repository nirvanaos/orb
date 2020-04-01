#include <Core/ORB/ProxyObject.h>
#include <Core/Runnable.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

using namespace ::Nirvana::Core;

class ProxyObject::Deactivator :
	public CoreDynamic <Runnable>
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
				run_garbage_collector (new Deactivator (*this));
			} catch (...) {
				// Async call failed, maybe resources are exausted.
				// Fallback to collect garbage in current thread.
				// TODO: Log the error
				try {
					Synchronized sync (sync_context ());
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

void ProxyObject::get_interface_request (ProxyObject* _servant, IORequest_ptr _rq,
	::Nirvana::ConstPointer _in_params,
	Unmarshal_var _unmarshal,
	::Nirvana::Pointer _out_params)
{
	InterfaceDef_var _ret = _servant->servant_->_get_interface ();
	get_interface_out& _out = *(get_interface_out*)_out_params;
	Marshal_ptr _m = _rq->marshaler ();
	_marshal_out (_ret, _m, _out._ret);
}

void ProxyObject::is_a_request (ProxyObject* _servant, IORequest_ptr _rq,
	::Nirvana::ConstPointer in_params,
	Unmarshal_var unmarshaler,
	::Nirvana::Pointer out_params)
{
	String logical_type_id;
	_unmarshal (((const is_a_in*)in_params)->logical_type_id, unmarshaler, logical_type_id);
	unmarshaler = Unmarshal::_nil ();
	Boolean _ret = _servant->servant_->_is_a (logical_type_id);
	BooleanRet& _out = *(BooleanRet*)out_params;
	_marshal_out (_ret, Marshal::_nil (), _out._ret);
}

void ProxyObject::non_existent_request (ProxyObject* _servant, IORequest_ptr _rq,
	::Nirvana::ConstPointer in_params,
	Unmarshal_var unmarshaler,
	::Nirvana::Pointer out_params)
{
	Boolean _ret = _servant->servant_->_non_existent ();
	BooleanRet& _out = *(BooleanRet*)out_params;
	Marshal_var _m;
	_marshal_out (_ret, Marshal::_nil (), _out._ret);
}

const Operation ProxyObject::object_ops_ [3] = {
	{ op_get_interface_, {0, 0}, {0, 0}, _tc_InterfaceDef, ObjProcWrapper <ProxyObject, get_interface_request> },
	{ op_is_a_, {&is_a_param_, 1}, {0, 0}, _tc_boolean, ObjProcWrapper <ProxyObject, is_a_request> },
	{ op_non_existent_, {0, 0}, {0, 0}, _tc_boolean, ObjProcWrapper <ProxyObject, non_existent_request> }
};

}
}
}
