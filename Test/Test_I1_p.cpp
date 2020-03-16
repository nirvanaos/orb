#include <CORBA/Proxy/Proxy.h>
#include "Test_I1_s.h"

namespace CORBA {
namespace Nirvana {

template <>
class ProxyFactoryImpl <::Test::I1> :
	public ProxyFactoryBase <::Test::I1>
{
public:
	static const InterfaceMetadata metadata_;

	// long op1 (long p1);

	typedef Long op1_in;
	typedef Long op1_out;
	static const Parameter op1_in_params_ [1];

	static void op1_platform_request (Interface* target,
		PlatformRequest_ptr call,
		::Nirvana::ConstPointer* in_params,
		PlatformUnmarshal_var unmarshaler,
		::Nirvana::Pointer* out_params)
	{
		try {

			const op1_in& in = *(const op1_in*)in_params;
			op1_out& out = *(op1_out*)out_params;
			out = Test::I1::_check (target)->op1 (in);

		} catch (const CORBA::UserException & e) {
			call->user_exception (e);
		} catch (const CORBA::SystemException & e) {
			call->system_exception (e);
		} catch (...) {
			call->unknown_exception ();
		}
	}

	// I1 object_op (I1 in_obj, out I1 out_obj, inout I1 inout_obj);

	struct object_op_in
	{
		Interface* in_obj;
		Interface* inout_obj;
	};
	static const Parameter object_op_in_params_ [2];

	struct object_op_out
	{
		Interface* out_obj;
		Interface* inout_obj;
		Interface* _ret;
	};
	static const Parameter object_op_out_params_ [2];

	static void object_op_platform_request (Interface* target,
		PlatformRequest_ptr _call,
		::Nirvana::ConstPointer* _in_ptr,
		PlatformUnmarshal_var _u,
		::Nirvana::Pointer* _out_ptr)
	{
		try {

			const object_op_in& _in = *(const object_op_in*)_in_ptr;
			
			// out and inout params
			I_var <::Test::I1> out_obj;
			I_var <::Test::I1> inout_obj;
			I_var <::Test::I1> _ret;
			_unmarshal (_in.inout_obj, _u, inout_obj);
			{
				// in params
				I_var <::Test::I1> in_obj;
				_unmarshal (_in.in_obj, _u, in_obj);

				// Release resources
				_u = PlatformUnmarshal::_nil ();

				// Call
				_ret = Test::I1::_check (target)->object_op (in_obj, out_obj, inout_obj);

				// Release input params data
			}

			// Marshal output
			object_op_out& _out = *(object_op_out*)_out_ptr;
			PlatformMarshal_var _m = _call->marshaler ();
			_marshal_out (out_obj, _m, _out.out_obj);
			_marshal_out (inout_obj, _m, _out.inout_obj);
			_marshal_out (_ret, _m, _out._ret);

		} catch (const CORBA::UserException & e) {
			_call->user_exception (e);
		} catch (const CORBA::SystemException & e) {
			_call->system_exception (e);
		} catch (...) {
			_call->unknown_exception ();
		}
	}

private:
	static const Operation operations_ [];
};

template <>
class PlatformProxy <::Test::I1> :
	public PlatformProxyBase <::Test::I1>
{
	typedef PlatformProxyBase <::Test::I1> Base;
	typedef ProxyFactoryImpl <::Test::I1> Factory;
public:
	PlatformProxy (PlatformObjRef_ptr proxy_manager, CORBA::UShort interface_idx) :
		Base (proxy_manager, interface_idx)
	{}

	Long op1 (Long p1) const
	{
		Factory::op1_out _out;
		PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 0 },
			&p1, sizeof (p1), _m, &_out, sizeof (_out));
		return _out;
	}

	void throw_no_implement () const
	{
		PlatformMarshal_var _m;
		_target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 1 },
			0, 0, _m, 0, 0);
	}

	::Test::I1_var object_op (::Test::I1_ptr in_obj, ::Test::I1_var& out_obj, ::Test::I1_var& inout_obj) const
	{
		Factory::object_op_in _in;
		PlatformMarshal_var _m = _target ()->create_marshaler ();
		_marshal_in (in_obj, _m, _in.in_obj);
		_marshal_in (inout_obj, _m, _in.inout_obj);
		Factory::object_op_out _out;
		PlatformUnmarshal_var _u = _target ()->call (CORBA::Nirvana::OperationIndex{ _interface_idx (), 2 },
			&_in, sizeof (_in), _m, &_out, sizeof (_out));
		_unmarshal (_out.out_obj, _u, out_obj);
		_unmarshal (_out.inout_obj, _u, inout_obj);
		::Test::I1_var _ret;
		_unmarshal (_out._ret, _u, _ret);
		return _ret;
	}

	String string_op (const String& in_s, String& out_s, String& inout_s) const;

	::Test::SeqLong seq_op (const ::Test::SeqLong& in_s, ::Test::SeqLong& out_s, ::Test::SeqLong& inout_s) const;

	Any any_op (const Any& in_any, Any& out_any, Any& inout_any) const;
};

const ::CORBA::Nirvana::Parameter ProxyFactoryImpl <::Test::I1>::op1_in_params_ [] {
	{ "p1", ::CORBA::_tc_long }
};

const Operation ProxyFactoryImpl <::Test::I1>::operations_ [] {
	{ "op1", { op1_in_params_, countof (op1_in_params_) }, {0, 0}, ::CORBA::_tc_long },
	{ "throw_no_implement", { 0, 0 }, {0, 0}, ::CORBA::_tc_void }
};

const InterfaceMetadata ProxyFactoryImpl <::Test::I1>::metadata_ {
	{0, 0},
  {operations_, countof (operations_)}
};

}
}

namespace Test {

typedef ::CORBA::Nirvana::ProxyFactoryImpl <I1> _I1_ProxyFactory;

}

NIRVANA_EXPORT_INTERFACE1 (Test, Test::I1::interface_id_, _I1_ProxyFactory, CORBA::Nirvana::ProxyFactory);
