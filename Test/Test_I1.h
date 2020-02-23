#ifndef IDL_TEST_I1_H_
#define IDL_TEST_I1_H_

#include <CORBA/CORBA.h>

namespace Test {

class I1;
typedef CORBA::Nirvana::I_ptr <I1> I1_ptr;
typedef CORBA::Nirvana::I_var <I1> I1_var;
typedef CORBA::Nirvana::I_out <I1> I1_out;

typedef CORBA::Nirvana::Sequence <CORBA::Long> SeqLong;
typedef CORBA::Nirvana::Type <SeqLong>::C_var SeqLong_var;
typedef CORBA::Nirvana::Type <SeqLong>::C_out SeqLong_out;
typedef CORBA::Nirvana::Type <SeqLong>::C_inout SeqLong_inout;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Test::I1)
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Long (*op1) (Bridge < ::Test::I1>*, Long p1, EnvironmentBridge*);
void (*throw_NO_IMPLEMENT) (Bridge < ::Test::I1>*, EnvironmentBridge*);
Interface* (*object_op) (Bridge < ::Test::I1>*, Interface* in_obj, Interface** out_obj, Interface** inout_obj, EnvironmentBridge*);
ABI_ret <String> (*string_op) (Bridge < ::Test::I1>*, ABI_in <String> in_s, ABI_out <String> out_s, ABI_inout <String> inout_s, EnvironmentBridge*);
ABI_ret < ::Test::SeqLong> (*seq_op) (Bridge < ::Test::I1>*, ABI_in < ::Test::SeqLong> in_s, ABI_out < ::Test::SeqLong> out_s, ABI_inout < ::Test::SeqLong> inout_s, EnvironmentBridge*);
ABI_ret <Any> (*any_op) (Bridge < ::Test::I1>*, ABI_in <Any>, ABI_out <Any>, ABI_inout <Any>, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Test::I1> :
	public T
{
public:
	Long op1 (Long p1);
	void throw_NO_IMPLEMENT ();
	::Test::I1_var object_op (I_in < ::Test::I1> in_obj, I_out < ::Test::I1> out_obj, I_inout < ::Test::I1> inout_obj);
	String string_op (CORBA::String_in, CORBA::String_out, CORBA::String_inout);
	::Test::SeqLong seq_op (Type <::Test::SeqLong>::C_in in_s, ::Test::SeqLong_out out_s, ::Test::SeqLong_inout inout_s);
	Any any_op (Any_in, Any_out, Any_inout);
};

template <class T>
Long Client <T, ::Test::I1>::op1 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.op1) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Test::I1>::throw_NO_IMPLEMENT ()
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.throw_NO_IMPLEMENT) (&_b, &_env);
	_env.check ();
}

template <class T>
::Test::I1_var Client <T, ::Test::I1>::object_op (I_in < ::Test::I1> in_obj, I_out < ::Test::I1> out_obj, I_inout < ::Test::I1> inout_obj)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	I_ret < ::Test::I1> _ret = (_b._epv ().epv.object_op) (&_b, &in_obj, &out_obj, &inout_obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
std::string Client <T, ::Test::I1>::string_op (CORBA::String_in in_s, CORBA::String_out out_s, CORBA::String_inout inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.string_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Test::SeqLong Client <T, ::Test::I1>::seq_op (Type <::Test::SeqLong>::C_in in_s, ::Test::SeqLong_out out_s, ::Test::SeqLong_inout inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <Sequence <Long> >::C_ret _ret = (_b._epv ().epv.seq_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Any Client <T, ::Test::I1>::any_op (Any_in in_any, Any_out out_any, Any_inout inout_any)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Type <Any>::C_ret _ret = (_b._epv ().epv.any_op) (&_b, &in_any, &out_any, &inout_any, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I1 : public CORBA::Nirvana::ClientInterface <I1, ::CORBA::Object>
{};

}

#endif
