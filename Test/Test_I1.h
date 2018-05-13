#ifndef NIRVANA_ORB_TEST_I1_H_
#define NIRVANA_ORB_TEST_I1_H_

#include "PortableServer.h"

namespace Test {

class I1;
typedef ::CORBA::Nirvana::T_ptr <I1> I1_ptr;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <::Test::I1> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge <::Test::I1>*, EnvironmentBridge*);
		}
		base;

		struct
		{
			Long (*op1) (Bridge <::Test::I1>*, Long p1, EnvironmentBridge*);
			void (*throw_NO_IMPLEMENT) (Bridge <::Test::I1>*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char* _primary_interface ()
	{
		return "IDL:Test/I1:1.0";
	}

	static Boolean ___is_a (const Char* id)
	{
		if (RepositoryId::compatible (_primary_interface (), id))
			return TRUE;
		// Here we must call all base classes
		return Bridge <Object>::___is_a (id);
	}

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, ::Test::I1> :
	public ClientBase <T, ::Test::I1>
{
public:
	Long op1 (Long p1);
	void throw_NO_IMPLEMENT ();
};

template <class T>
Long Client <T, ::Test::I1>::op1 (Long p1)
{
	Environment _env;
	Bridge <::Test::I1>& _b = ClientBase <T, ::Test::I1>::_bridge ();
	Long _ret = (_b._epv ().epv.op1) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Test::I1>::throw_NO_IMPLEMENT ()
{
	Environment _env;
	Bridge <::Test::I1>& _b = ClientBase <T, ::Test::I1>::_bridge ();
	(_b._epv ().epv.throw_NO_IMPLEMENT) (&_b, &_env);
	_env.check ();
}

}
}

namespace Test {

class I1 :
	public CORBA::Nirvana::ClientInterface <I1>,
	public CORBA::Nirvana::Client <I1, ::CORBA::Object>
{
public:
	typedef I1_ptr _ptr_type;

	operator ::CORBA::Object& ()
	{
		::CORBA::Environment _env;
		::CORBA::Object* _ret = static_cast <::CORBA::Object*> ((_epv ().base.CORBA_Object) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

}

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I1>
{
public:
	static const typename Bridge <::Test::I1>::EPV sm_epv;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <::Test::I1>::_primary_interface (), id))
			return &S::_narrow <::Test::I1> (base);
		else
			return Skeleton <S, Object>::_find_interface (base, id);
	}

protected:
	static Long _op1 (Bridge <::Test::I1>* _b, Long p1, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).op1 (p1);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}

	static void _throw_NO_IMPLEMENT (Bridge <::Test::I1>* _b, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).throw_NO_IMPLEMENT ();
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
	}
};

template <class S>
const Bridge <::Test::I1>::EPV Skeleton <S, ::Test::I1>::sm_epv = {
	{ // interface
		S::_duplicate <::Test::I1>,
		S::_release <::Test::I1>
	},
	{ // base
		S::_wide <Object, ::Test::I1>
	},
	{ // epv
		S::_op1,
		S::_throw_NO_IMPLEMENT
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I1> :
	public Implementation <S, ::Test::I1, ::CORBA::Object>
{};

// POA implementation

template <>
class ServantPOA <::Test::I1> :
	public ImplementationPOA <::Test::I1, Object>
{
public:
	virtual Long op1 (Long p1) = 0;
	virtual void throw_NO_IMPLEMENT () = 0;
};

// Static implementation

template <class S>
class ServantStatic <S, ::Test::I1> :
	public ImplementationStatic <S, ::Test::I1, ::CORBA::Object>
{};

// Tied implementation

template <class T>
class ServantTied <T, ::Test::I1>	: 
	public ImplementationTied <T, ::Test::I1, ::CORBA::Object>
{
public:
	ServantTied (T* tp, Boolean release) :
		ImplementationTied <T, ::Test::I1, ::CORBA::Object> (tp, release)
	{}
};

}
}

namespace POA_Test {

typedef ::CORBA::Nirvana::ServantPOA <::Test::I1> I1;

template <class T>
class I1_tie :
	public ::CORBA::Nirvana::ServantTied <T, ::Test::I1>
{
public:
	I1_tie (T& t) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I1> (&t, FALSE)
	{}

	I1_tie (T& t, ::PortableServer::POA_ptr poa);	// Generate only if Object is derived

	I1_tie (T* tp, ::CORBA::Boolean release = TRUE) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I1> (tp, release)
	{}

	I1_tie (T* tp, ::PortableServer::POA_ptr poa, ::CORBA::Boolean release = TRUE);	// Generate only if Object is derived
};

}

#endif
