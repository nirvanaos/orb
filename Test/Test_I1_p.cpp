#include <CORBA/Proxy/Proxy.h>
#include "Test_I1_s.h"

namespace Test {

class _ProxyFactory_I1 :
	public CORBA::Nirvana::ServantStatic <_ProxyFactory_I1, CORBA::Nirvana::ProxyFactory>
{
public:
	inline CORBA::Nirvana::Interface_ptr create_platform_proxy (
		CORBA::Nirvana::PlatformObjRef_ptr proxy_manager,
		CORBA::UShort interface_idx, CORBA::AbstractBase_ptr bases,
		CORBA::Nirvana::DynamicServant_ptr& deleter);

	typedef CORBA::Long _op1_in;
	typedef CORBA::Long _op1_out;

	static CORBA::Nirvana::PlatformMarshal_var _op1_PlatformRequest (CORBA::Nirvana::Interface* target,
		CORBA::Nirvana::PlatformRequest_ptr call,
		::Nirvana::ConstPointer* in_params,
		CORBA::Nirvana::PlatformUnmarshal_var unmarshaler,
		::Nirvana::Pointer* out_params)
	{
		try {
			const _op1_in& in = *(const _op1_in*)in_params;
			_op1_out& out = *(_op1_out*)out_params;
			out = I1::_check (target)->op1 (in);
			return CORBA::Nirvana::PlatformMarshal::_nil ();
		} catch (const CORBA::UserException& e) {
			call->user_exception (e);
		} catch (const CORBA::SystemException & e) {
			call->system_exception (e);
		} catch (...) {
			call->unknown_exception ();
		}
	}

	struct _object_op_in
	{
		CORBA::Nirvana::Interface* in_obj;
		CORBA::Nirvana::Interface* inout_obj;
	};

	struct _object_op_out
	{
		CORBA::Nirvana::Interface* out_obj;
		CORBA::Nirvana::Interface* inout_obj;
		CORBA::Nirvana::Interface* _ret;
	};
};

class _PlatformProxy_I1 :
	public CORBA::Nirvana::PlatformProxyBaseT <_PlatformProxy_I1, I1>
{
	typedef CORBA::Nirvana::PlatformProxyBaseT <_PlatformProxy_I1, I1> Base;
public:
	_PlatformProxy_I1 (CORBA::Nirvana::PlatformObjRef_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		_ProxyFactory_I1::_op1_out _out;
		CORBA::Nirvana::PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 0 },
			&p1, sizeof (p1), _m, &_out, sizeof (_out));
		return _out;
	}

	void throw_NO_IMPLEMENT () const
	{
		CORBA::Nirvana::PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 1 },
			0, 0, _m, 0, 0);
	}

	I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj) const
	{
		_ProxyFactory_I1::_object_op_in _in;
		::CORBA::Nirvana::PlatformMarshal_var _m = _target ()->create_marshaler ();
		::CORBA::Nirvana::MarshalTraits <I1_var>::marshal_in (in_obj, _m, _in.in_obj);
		::CORBA::Nirvana::MarshalTraits <I1_var>::marshal_in (inout_obj, _m, _in.inout_obj);
		_ProxyFactory_I1::_object_op_out _out;
		::CORBA::Nirvana::PlatformUnmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 2 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out.out_obj, _u, out_obj);
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out.inout_obj, _u, inout_obj);
		I1_var _ret;
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	CORBA::Nirvana::String string_op (const CORBA::Nirvana::String& in_s, CORBA::Nirvana::String& out_s, CORBA::Nirvana::String& inout_s) const;

	std::vector <CORBA::Long> seq_op (const SeqLong& in_s, SeqLong& out_s, SeqLong& inout_s) const;

	CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any) const;
};

inline
CORBA::Nirvana::Interface_ptr _ProxyFactory_I1::create_platform_proxy (
	CORBA::Nirvana::PlatformObjRef_ptr proxy_manager,
	CORBA::UShort interface_idx, CORBA::AbstractBase_ptr bases,
	CORBA::Nirvana::DynamicServant_ptr& deleter)
{
	_PlatformProxy_I1* proxy =
		Nirvana::stateless_create <_PlatformProxy_I1> (proxy_manager, interface_idx);
	deleter = proxy->_dynamic_servant ();
	return proxy->_proxy ();
}

}
