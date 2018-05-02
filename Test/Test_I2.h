#ifndef NIRVANA_ORB_TEST_I2_H_
#define NIRVANA_ORB_TEST_I2_H_

#include "PortableServer.h"

namespace Test {

class I2;
typedef ::CORBA::Nirvana::T_ptr <I2> I2_ptr;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <::Test::I2> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge <::Test::I2>*, Environment*);
		}
		base;

		struct
		{
			Long (*op2) (Bridge <::Test::I2>*, Long p1, Environment*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char* _primary_interface ()
	{
		return "IDL:Test/I2:1.0";
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
	Bridge <::Test::I2>& _b = _bridge ();
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
class Skeleton <S, ::Test::I2>
{
public:
	static const typename Bridge <::Test::I2>::EPV sm_epv;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <::Test::I2>::_primary_interface (), id))
			return &S::_narrow <::Test::I2> (base);
		else
			return Skeleton <S, Object>::_find_interface (base, id);
	}

protected:
	static Long _op2 (Bridge <::Test::I2>* _b, Long p1, Environment* _env)
	{
		try {
			return S::_implementation (_b).op2 (p1);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <::Test::I2>::EPV Skeleton <S, ::Test::I2>::sm_epv = {
	{ // interface
		S::_duplicate <::Test::I2>,
		S::_release <::Test::I2>
	},
	{ // base
		S::_wide <Object, ::Test::I2>
	},
	{ // epv
		S::_op2
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I2> :
	public Implementation <S, ::Test::I2, ::CORBA::Object>
{};

// POA implementation

template <>
class ServantPOA <::Test::I2> :
	public ImplementationPOA <::Test::I2, Object>
{
public:
	virtual Long op2 (Long p1) = 0;
};

// Static implementation

template <class S>
class ServantStatic <S, ::Test::I2> :
	public ImplementationStatic <S, ::Test::I2, ::CORBA::Object>
{};

// Tied implementation

template <class T>
class ServantTied <T, ::Test::I2>	: 
	public ImplementationTied <T, ::Test::I2, ::CORBA::Object>
{
public:
	ServantTied (T& t) :
		ImplementationTied (&t, FALSE)
	{}
	
	ServantTied (T& t, POA_ptr poa);	// TODO: Implement

	ServantTied (T* tp, Boolean release = TRUE) :
		ImplementationTied (tp, release)
	{}

	ServantTied (T* tp, POA_ptr poa, Boolean release = TRUE);	// TODO: Implement
};

}
}

namespace POA_Test {

typedef ::CORBA::Nirvana::ServantPOA <::Test::I2> I2;

template <class T>
using I2_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I2>;

}

#endif