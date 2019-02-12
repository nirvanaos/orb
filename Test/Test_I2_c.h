#ifndef NIRVANA_ORB_TEST_I2_C_H_
#define NIRVANA_ORB_TEST_I2_C_H_

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
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge < ::Test::I2>*, EnvironmentBridge*);
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
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, ::Test::I2> :
	public ClientBase <T, ::Test::I2>
{
public:
	Long op2 (Long p1);
};

template <class T>
Long Client <T, ::Test::I2>::op2 (Long p1)
{
	Environment _env;
	Bridge < ::Test::I2>& _b = ClientBase <T, ::Test::I2>::_bridge ();
	Long _ret = (_b._epv ().epv.op2) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I2 :
	public CORBA::Nirvana::ClientInterface <I2>,
	public CORBA::Nirvana::Client <I2, ::CORBA::Object>
{
public:
	typedef I2_ptr _ptr_type;

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
