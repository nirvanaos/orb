#include <CORBA/Proxy/Proxy.h>
#include "Test_I1_s.h"

namespace Test {

class _I1_ProxyFactory :
	public ::CORBA::Nirvana::ServantStatic <_I1_ProxyFactory, ::CORBA::Nirvana::ProxyFactory>
{
public:
	static const ::CORBA::Nirvana::CountedArray <const ::CORBA::Char*> base_interfaces_;
	static const ::CORBA::Nirvana::Operation operations_a_ [];
	static const ::CORBA::Nirvana::CountedArray <::CORBA::Nirvana::Operation> operations_;

	inline ::CORBA::Nirvana::Interface_ptr create_platform_proxy (
		::CORBA::Nirvana::PlatformObjRef_ptr proxy_manager, ::CORBA::UShort interface_idx,
		::CORBA::Nirvana::DynamicServant_var& deleter);

	typedef ::CORBA::Long op1_in;
	typedef ::CORBA::Long op1_out;
	static const ::CORBA::Nirvana::Parameter op1_in_params_a_ [];
	static const ::CORBA::Nirvana::CountedArray <::CORBA::Nirvana::Parameter> op1_in_params_;

	static ::CORBA::Nirvana::PlatformMarshal_var op1_PlatformRequest (::CORBA::Nirvana::Interface* target,
		::CORBA::Nirvana::PlatformRequest_ptr call,
		::Nirvana::ConstPointer* in_params,
		::CORBA::Nirvana::PlatformUnmarshal_var unmarshaler,
		::Nirvana::Pointer* out_params)
	{
		try {
			const op1_in& in = *(const op1_in*)in_params;
			op1_out& out = *(op1_out*)out_params;
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

	struct object_op_in
	{
		::CORBA::Nirvana::Interface* in_obj;
		::CORBA::Nirvana::Interface* inout_obj;
	};

	struct object_op_out
	{
		::CORBA::Nirvana::Interface* out_obj;
		::CORBA::Nirvana::Interface* inout_obj;
		::CORBA::Nirvana::Interface* _ret;
	};
};

class _I1_PlatformProxy :
	public ::CORBA::Nirvana::PlatformProxyBaseT <_I1_PlatformProxy, I1>
{
	typedef ::CORBA::Nirvana::PlatformProxyBaseT <_I1_PlatformProxy, I1> Base;
public:
	_I1_PlatformProxy (CORBA::Nirvana::PlatformObjRef_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		_I1_ProxyFactory::op1_out _out;
		::CORBA::Nirvana::PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 0 },
			&p1, sizeof (p1), _m, &_out, sizeof (_out));
		return _out;
	}

	void throw_no_implement () const
	{
		::CORBA::Nirvana::PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 1 },
			0, 0, _m, 0, 0);
	}

	I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj) const
	{
		_I1_ProxyFactory::object_op_in _in;
		::CORBA::Nirvana::PlatformMarshal_var _m = _target ()->create_marshaler ();
		::CORBA::Nirvana::MarshalTraits <I1_var>::marshal_in (in_obj, _m, _in.in_obj);
		::CORBA::Nirvana::MarshalTraits <I1_var>::marshal_in (inout_obj, _m, _in.inout_obj);
		_I1_ProxyFactory::object_op_out _out;
		::CORBA::Nirvana::PlatformUnmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 2 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out.out_obj, _u, out_obj);
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out.inout_obj, _u, inout_obj);
		I1_var _ret;
		::CORBA::Nirvana::MarshalTraits <I1_var>::unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	::CORBA::Nirvana::String string_op (const ::CORBA::Nirvana::String& in_s, ::CORBA::Nirvana::String& out_s, ::CORBA::Nirvana::String& inout_s) const;

	SeqLong seq_op (const SeqLong& in_s, SeqLong& out_s, SeqLong& inout_s) const;

	::CORBA::Any any_op (const ::CORBA::Any& in_any, ::CORBA::Any& out_any, ::CORBA::Any& inout_any) const;
};

const ::CORBA::Nirvana::CountedArray <const ::CORBA::Char*> _I1_ProxyFactory::base_interfaces_{ 0, 0 };

const ::CORBA::Nirvana::Parameter _I1_ProxyFactory::op1_in_params_a_ []{
	{ "p1", ::CORBA::_tc_long }
};

const ::CORBA::Nirvana::Operation _I1_ProxyFactory::operations_a_ [] = {
	{ "op1", { op1_in_params_a_, countof (op1_in_params_a_) }, {0, 0}, ::CORBA::_tc_long },
	{ "throw_no_implement", { 0, 0 }, {0, 0}, ::CORBA::_tc_void }
};

inline
::CORBA::Nirvana::Interface_ptr _I1_ProxyFactory::create_platform_proxy (
	::CORBA::Nirvana::PlatformObjRef_ptr proxy_manager, ::CORBA::UShort interface_idx,
	::CORBA::Nirvana::DynamicServant_var& deleter)
{
	_I1_PlatformProxy* proxy =
		::Nirvana::stateless_create <_I1_PlatformProxy> (proxy_manager, interface_idx);
	deleter = proxy->_dynamic_servant ();
	return proxy->_proxy ();
}

}

NIRVANA_EXPORT_INTERFACE1 (Test, _I1_ProxyFactory, Test::_I1_ProxyFactory, CORBA::Nirvana::ProxyFactory);
