#ifndef IDL_TEST_I1_H_
#define IDL_TEST_I1_H_

#include <CORBA/CORBA.h>

namespace Test {

class I1;
typedef ::CORBA::Nirvana::T_ptr <I1> I1_ptr;
typedef ::CORBA::Nirvana::T_var <I1> I1_var;
typedef ::CORBA::Nirvana::T_out <I1> I1_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Test::I1> :
	public BridgeMarshal < ::Test::I1>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
		}
		base;

		struct
		{
			Long (*op1) (Bridge < ::Test::I1>*, Long p1, EnvironmentBridge*);
			void (*throw_NO_IMPLEMENT) (Bridge < ::Test::I1>*, EnvironmentBridge*);
			BridgeMarshal < ::Test::I1>* (*object_op) (Bridge < ::Test::I1>*, BridgeMarshal < ::Test::I1>* in_obj, BridgeMarshal < ::Test::I1>** out_obj, BridgeMarshal < ::Test::I1>** inout_obj, EnvironmentBridge*);
			StringABI <char> (*string_op) (Bridge < ::Test::I1>*, const StringABI <char>* in_s, StringABI <char>* out_s, StringABI <char>* inout_s, EnvironmentBridge*);
			StringABI <char> (*bstring_op) (Bridge < ::Test::I1>*, const StringABI <char>* in_s, StringABI <char>* out_s, StringABI <char>* inout_s, EnvironmentBridge*);
			SequenceABI <Long> (*seq_op) (Bridge < ::Test::I1>*, const SequenceABI <Long>* in_s, SequenceABI <Long>* out_s, SequenceABI <Long>* inout_s, EnvironmentBridge*);
			ABI <Any>::ABI_ret (*any_op) (Bridge < ::Test::I1>*, ABI <Any>::ABI_in, ABI <Any>::ABI_out, ABI <Any>::ABI_inout, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal < ::Test::I1> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Test::I1> :
	public T
{
public:
	Long op1 (Long p1);
	void throw_NO_IMPLEMENT ();
	T_ptr < ::Test::I1> object_op (T_ptr < ::Test::I1> in_obj, T_out < ::Test::I1> out_obj, T_inout < ::Test::I1> inout_obj);
	std::string string_op (String_in <char> in_s, String_out <char> out_s, String_inout <char> inout_s);
	std::string bstring_op (String_in <char> in_s, String_out <char> out_s, String_inout <char> inout_s);
	std::vector <Long> seq_op (Sequence_in <Long> in_s, Sequence_out <Long> out_s, Sequence_inout <Long> inout_s);
	CORBA::Any any_op (ABI <CORBA::Any>::C_in, ABI <CORBA::Any>::C_out, ABI <CORBA::Any>::C_inout);
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
T_ptr < ::Test::I1> Client <T, ::Test::I1>::object_op (T_ptr < ::Test::I1> in_obj, T_out < ::Test::I1> out_obj, T_inout < ::Test::I1> inout_obj)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	T_var < ::Test::I1> _ret = (_b._epv ().epv.object_op) (&_b, in_obj, &out_obj, &inout_obj, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
std::string Client <T, ::Test::I1>::string_op (String_in <char> in_s, String_out <char> out_s, String_inout <char> inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	String_var <char> _ret = (_b._epv ().epv.string_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
std::string Client <T, ::Test::I1>::bstring_op (String_in <char> in_s, String_out <char> out_s, String_inout <char> inout_s)
{
	_check_bound (in_s, 20);
	_check_bound (inout_s, 20);
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	String_var <char> _ret = (_b._epv ().epv.bstring_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	_check_bound (_ret, 20);
	return _ret._retn ();
}

template <class T>
std::vector <Long> Client <T, ::Test::I1>::seq_op (Sequence_in <Long> in_s, Sequence_out <Long> out_s, Sequence_inout <Long> inout_s)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	Sequence_var <Long> _ret = (_b._epv ().epv.seq_op) (&_b, &in_s, &out_s, &inout_s, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
CORBA::Any Client <T, ::Test::I1>::any_op (ABI <CORBA::Any>::C_in in_any, ABI <CORBA::Any>::C_out out_any, ABI <CORBA::Any>::C_inout inout_any)
{
	Environment _env;
	Bridge < ::Test::I1>& _b (T::_get_bridge (_env));
	ABI <CORBA::Any>::C_ret _ret = (_b._epv ().epv.seq_op) (&_b, in_any, out_any, inout_any, &_env);
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
