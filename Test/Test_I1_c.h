#ifndef IDL_TEST_I1_C_H_
#define IDL_TEST_I1_C_H_

#include <Object_c.h>

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

}
}

namespace Test {

class I1 : public CORBA::Nirvana::ClientInterface <I1, ::CORBA::Object>
{};

}

#endif
