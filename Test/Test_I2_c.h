#ifndef IDL_TEST_I2_C_H_
#define IDL_TEST_I2_C_H_

#include <Object_c.h>

namespace Test {

class I2;
typedef ::CORBA::Nirvana::T_ptr <I2> I2_ptr;
typedef ::CORBA::Nirvana::T_var <I2> I2_var;
typedef ::CORBA::Nirvana::T_out <I2> I2_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Test::I2> :
	public BridgeMarshal < ::Test::I2>
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
			Long (*op2) (Bridge < ::Test::I2>*, Long p1, EnvironmentBridge*);
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
		BridgeMarshal < ::Test::I2> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Test::I2> :
	public T
{
public:
	Long op2 (Long p1);
};

template <class T>
Long Client <T, ::Test::I2>::op2 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I2>& _b (*this);
	Long _ret = (_b._epv ().epv.op2) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I2 :
	public CORBA::Nirvana::ClientInterface <I2>,
	public CORBA::Nirvana::ClientInterfaceBase <I2, ::CORBA::Object>
{};

}

#endif
