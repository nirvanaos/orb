#ifndef IDL_TEST_I1_S_H_
#define IDL_TEST_I1_S_H_

#include "Test_I1.h"
#include "Server.h"

#ifdef TEST_LOCAL_OBJECT
typedef CORBA::LocalObject TestObjectBase;
#else
typedef PortableServer::ServantBase TestObjectBase;
#endif

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I1>
{
public:
	static const typename Bridge < ::Test::I1>::EPV epv_;

protected:
	static Long _op1 (Bridge < ::Test::I1>* _b, Long p1, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).op1 (p1);
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _throw_NO_IMPLEMENT (Bridge < ::Test::I1>* _b, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).throw_NO_IMPLEMENT ();
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Interface* _object_op (Bridge < ::Test::I1>* _b, Interface* in_obj, Interface** out_obj, Interface** inout_obj, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).object_op (TypeI < ::Test::I1>::in (in_obj), TypeI < ::Test::I1>::out (out_obj), TypeI < ::Test::I1>::inout (inout_obj));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ABI_ret <String> _string_op (Bridge < ::Test::I1>* _b, ABI_in <String> in_s, ABI_out <String> out_s, ABI_inout <String> inout_s, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).string_op (Type <String>::in (in_s), Type <String>::out (out_s), Type <String>::inout (inout_s));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return ABI_ret <std::string> ();
	}

	static ABI_ret < ::Test::SeqLong> _seq_op (Bridge < ::Test::I1>* _b, ABI_in < ::Test::SeqLong> in_s, ABI_out < ::Test::SeqLong> out_s, ABI_inout < ::Test::SeqLong> inout_s, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).seq_op (Type < ::Test::SeqLong>::in (in_s), Type < ::Test::SeqLong>::out (out_s), Type < ::Test::SeqLong>::inout (inout_s));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return ABI_ret <std::vector <Long> > ();
	}

	static ABI_ret <Any> _any_op (Bridge < ::Test::I1>* _b, ABI_in <Any> in_any, ABI_out <Any> out_any, ABI_inout <Any> inout_any, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).any_op (Type <Any>::in (in_any), Type <Any>::out (out_any), Type <Any>::inout (inout_any));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return ABI_ret <Any> ();
	}
};

template <class S>
const Bridge < ::Test::I1>::EPV Skeleton <S, ::Test::I1>::epv_ = {
	{ // interface
		Bridge < ::Test::I1>::interface_id_,
		S::template __duplicate < ::Test::I1>,
		S::template __release < ::Test::I1>
	},
	{ // base
		S::template _wide <Object, ::Test::I1>
	},
	{ // epv
		S::_op1,
		S::_throw_NO_IMPLEMENT,
		S::_object_op,
		S::_string_op,
		S::_seq_op,
		S::_any_op
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I1> :
	public Implementation <S, ::Test::I1, TestObjectBase>
{};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA < ::Test::I1> :
	public ImplementationPOA < ::Test::I1, TestObjectBase>
{
public:
	virtual Long op1 (Long p1) = 0;
	virtual void throw_NO_IMPLEMENT () = 0;
	virtual ::Test::I1_ptr object_op (::Test::I1_ptr in_obj, ::Test::I1_var& out_obj, ::Test::I1_var& inout_obj) = 0;
	virtual std::string string_op (const std::string&, std::string&, std::string&) = 0;
	virtual std::vector <Long> seq_op (const std::vector <Long>& in_s, std::vector <Long>& out_s, std::vector <Long>& inout_s) = 0;
	virtual CORBA::Any any_op (const CORBA::Any&, CORBA::Any&, CORBA::Any&) = 0;
};

}
}

namespace POA_Test {
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I1> I1;
}

#endif

// Static implementation
#ifndef TEST_NO_STATIC

namespace CORBA {
namespace Nirvana {

template <class S>
class ServantStatic <S, ::Test::I1> :
	public ImplementationStatic <S, ::Test::I1, TestObjectBase>
{};

}
}

#endif

// Tied implementation
#ifndef TEST_NO_TIED

namespace POA_Test {

template <class T> using I1_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I1>;

}

#endif

#endif
