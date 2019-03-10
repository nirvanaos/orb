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
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge < ::Test::I1>*, const Char* id, EnvironmentBridge*);
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
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class ClientBase <T, ::Test::I1>
{
public:
	operator ::Test::I1& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge < ::Test::I1>* _ret = (t._epv ().base.Test_I1) (&t, Bridge < ::Test::I1>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast < ::Test::I1&> (*_ret);
	}

	operator Bridge < ::Test::I1>& ()
	{
		return operator ::Test::I1& ();
	}
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
	Bridge < ::Test::I1>& _b (*this);
	Long _ret = (_b._epv ().epv.op1) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Test::I1>::throw_NO_IMPLEMENT ()
{
	Environment _env;
	Bridge < ::Test::I1>& _b (*this);
	(_b._epv ().epv.throw_NO_IMPLEMENT) (&_b, &_env);
	_env.check ();
}

}
}

namespace Test {

class I1 :
	public CORBA::Nirvana::ClientInterface <I1>,
	public CORBA::Nirvana::ClientInterfaceBase <I1, ::CORBA::Object>
{
public:
	typedef I1_ptr _ptr_type;
};

}

#endif
