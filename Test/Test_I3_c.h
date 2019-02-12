#ifndef NIRVANA_ORB_TEST_I3_C_H_
#define NIRVANA_ORB_TEST_I3_C_H_

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
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge < ::Test::I3>*, EnvironmentBridge*);
			Bridge < ::Test::I1>* (*Test_I1) (Bridge < ::Test::I3>*, EnvironmentBridge*);
			Bridge < ::Test::I2>* (*Test_I2) (Bridge < ::Test::I3>*, EnvironmentBridge*);
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
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, ::Test::I3> :
	public ClientBase <T, ::Test::I3>
{
public:
	Long op3 (Long p1);
};

template <class T>
Long Client <T, ::Test::I3>::op3 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I3>& _b = ClientBase <T, ::Test::I3>::_bridge ();
	Long _ret = (_b._epv ().epv.op3) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I3 :
	public CORBA::Nirvana::ClientInterface <I3>,
	public CORBA::Nirvana::Client <I3, ::Test::I1>,
	public CORBA::Nirvana::Client <I3, ::Test::I2>,
	public CORBA::Nirvana::Client <I3, ::CORBA::Object>
{
public:
	typedef I3_ptr _ptr_type;

	operator ::Test::I1& ()
	{
		::CORBA::Environment _env;
		::Test::I1* _ret = static_cast < ::Test::I1*> ((_epv ().base.Test_I1) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}

	operator ::Test::I2& ()
	{
		::CORBA::Environment _env;
		::Test::I2* _ret = static_cast < ::Test::I2*> ((_epv ().base.Test_I2) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}

	operator ::CORBA::Object& ()
	{
		::CORBA::Environment _env;
		::CORBA::Object* _ret = static_cast < ::CORBA::Object*> ((_epv ().base.CORBA_Object) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

}

#endif
