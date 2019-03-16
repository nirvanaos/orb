#ifndef IDL_TEST_I3_C_H_
#define IDL_TEST_I3_C_H_

#include "Test_I1_c.h"
#include "Test_I2_c.h"

namespace Test {

class I3;
typedef ::CORBA::Nirvana::T_ptr <I3> I3_ptr;
typedef ::CORBA::Nirvana::T_var <I3> I3_var;
typedef ::CORBA::Nirvana::T_out <I3> I3_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Test::I3> :
	public BridgeMarshal < ::Test::I3>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
			BASE_STRUCT_ENTRY (::Test::I1, Test_I1)
			BASE_STRUCT_ENTRY (::Test::I2, Test_I2)
		}
		base;

		struct
		{
			Long (*op3) (Bridge < ::Test::I3>*, Long p1, EnvironmentBridge*);
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
		BridgeMarshal < ::Test::I3> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Test::I3> :
	public T
{
public:
	Long op3 (Long p1);
};

template <class T>
Long Client <T, ::Test::I3>::op3 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I3>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.op3) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I3 : public CORBA::Nirvana::ClientInterface <I3, I2, I1, ::CORBA::Object>
{};

}

#endif
