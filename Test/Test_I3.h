#ifndef NIRVANA_ORB_TEST_I3_H_
#define NIRVANA_ORB_TEST_I3_H_

#include "Test_I1.h"
#include "Test_I2.h"

namespace Test {

class I3;
typedef ::CORBA::Nirvana::T_ptr <I3> I3_ptr;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <::Test::I3> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge <::Test::I3>*, Environment*);
			Bridge <::Test::I1>* (*Test_I1) (Bridge <::Test::I3>*, Environment*);
			Bridge <::Test::I2>* (*Test_I2) (Bridge <::Test::I3>*, Environment*);
		}
		base;

		struct
		{
			Long (*op3) (Bridge <::Test::I3>*, Long p1, Environment*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char* _primary_interface ()
	{
		return "IDL:Test/I3:1.0";
	}

	static Boolean ___is_a (const Char* id)
	{
		if (RepositoryId::compatible (_primary_interface (), id))
			return TRUE;
		// Here we must call all direct base classes
		if (Bridge <::Test::I1>::___is_a (id))
			return TRUE;
		return Bridge <::Test::I2>::___is_a (id);
	}

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
	Bridge <::Test::I3>& _b = _bridge ();
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
		::Test::I1* _ret = static_cast <::Test::I1*> ((_epv ().base.Test_I1) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}

	operator ::Test::I2& ()
	{
		::CORBA::Environment _env;
		::Test::I2* _ret = static_cast <::Test::I2*> ((_epv ().base.Test_I2) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}

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
class Skeleton <S, ::Test::I3>
{
public:
	static const typename Bridge <::Test::I3>::EPV sm_epv;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		Bridge <Interface>* itf;
		if (RepositoryId::compatible (Bridge <::Test::I3>::_primary_interface (), id))
			itf = &S::_narrow <::Test::I3> (base);
		// Call all direct bases
		else if (!(itf = Skeleton <S, ::Test::I2>::_find_interface (base, id)))
			itf = Skeleton <S, ::Test::I1>::_find_interface (base, id);
		return itf;
	}

protected:
	static Long _op3 (Bridge <::Test::I3>* _b, Long p1, Environment* _env)
	{
		try {
			return S::_implementation (_b).op3 (p1);
		} catch (const Exception& e) {
			_env->set_exception (e);
		} catch (...) {
			_env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <::Test::I3>::EPV Skeleton <S, ::Test::I3>::sm_epv = {
	{ // interface
		S::_duplicate <::Test::I2>,
		S::_release <::Test::I2>
	},
	{ // base
		S::_wide <Object, ::Test::I3>,
		S::_wide <::Test::I1, ::Test::I3>,
		S::_wide <::Test::I2, ::Test::I3>
	},
	{ // epv
		S::_op3
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I3> :
	public Implementation <S, ::Test::I3, ::Test::I2, ::Test::I1, ::CORBA::Object>
{};

// POA implementation

template <>
class ServantPOA <::Test::I3> :
	public ImplementationPOA <::Test::I3, ::Test::I2, ::Test::I1>
{
public:
	virtual Long op3 (Long p1) = 0;
};

// Static implementation

template <class S>
class ServantStatic <S, ::Test::I3> :
	public ImplementationStatic <S, ::Test::I3, ::Test::I2, ::Test::I1, ::CORBA::Object>
{};

// Tied implementation

template <class T>
class ServantTied <T, ::Test::I3> :
	public ImplementationTied <T, ::Test::I3, ::Test::I2, ::Test::I1, ::CORBA::Object>
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

typedef ::CORBA::Nirvana::ServantPOA <::Test::I3> I3;

template <class T>
using I3_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I3>;

}

#endif