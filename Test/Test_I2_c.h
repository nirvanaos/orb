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
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge < ::Test::I2>*, const Char*, EnvironmentBridge*);
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
};

template <class T>
class ClientBase <T, ::Test::I2>
{
public:
	operator ::Test::I2& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge < ::Test::I2>* _ret = (t._epv ().base.Test_I2) (&t, Bridge < ::Test::I2>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <::Test::I2&> (*_ret);
	}

	operator Bridge < ::Test::I2>& ()
	{
		return operator ::Test::I2& ();
	}
};

template <>
class ClientBase < ::Test::I2, ::Test::I2> :
	public ClientBridge < ::Test::I2>
{};

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
	public CORBA::Nirvana::Client <I2, ::CORBA::Object>
{
public:
	typedef I2_ptr _ptr_type;
};

}

#endif
