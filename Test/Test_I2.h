#ifndef NIRVANA_ORB_TEST_I2_H_
#define NIRVANA_ORB_TEST_I2_H_

#include <ORB.h>

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

const Char Bridge < ::Test::I2>::interface_id_ [] = "IDL:Test/I2:1.0";

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
	typedef I1_ptr _ptr_type;

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

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I2>
{
public:
	static const typename Bridge < ::Test::I2>::EPV epv_;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge < ::Test::I2>::interface_id_, id))
			return &S::template _narrow < ::Test::I2> (base);
		else
			return Skeleton <S, Object>::_find_interface (base, id);
	}

protected:
	static Long _op2 (Bridge < ::Test::I2>* _b, Long p1, EnvironmentBridge* _env)
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
const Bridge < ::Test::I2>::EPV Skeleton <S, ::Test::I2>::epv_ = {
	{ // interface
		S::template _duplicate < ::Test::I2>,
		S::template _release < ::Test::I2>
	},
	{ // base
		S::template _wide <Object, ::Test::I2>
	},
	{ // epv
		S::_op2
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I2> :
	public AbstractBaseImpl <S>,
	public ObjectImpl <S, ::Test::I2>,
	public InterfaceImpl <S, ::Test::I2>
{
public:
	static Bridge <Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return Skeleton <S, ::Test::I2>::_find_interface (base, id);
	}
};

// POA implementation
#ifndef TEST_NO_POA
template <>
class ServantPOA < ::Test::I2> :
	public virtual ObjectPOA,
	public InterfaceImpl <ServantPOA < ::Test::I2>, ::Test::I2>
{
public:
	virtual const Char* _primary_interface ()
	{
		return Bridge < ::Test::I2>::interface_id_;
	}

	virtual Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA < ::Test::I2>, ::Test::I2>::_find_interface (*this, id);
	}

	T_ptr < ::Test::I2> _this ()
	{
		ServantBaseImpl::_activate ();
		return this;
	}

	virtual Long op2 (Long p1) = 0;
};
#endif

// Static implementation
#ifndef TEST_NO_STATIC
template <class S>
class ServantStatic <S, ::Test::I2> :
	public AbstractBaseStatic <S>,
	public ObjectStatic <S, ::Test::I2>,
	public InterfaceStatic <S, ::Test::I2>
{
public:
	static Bridge <Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return Skeleton <S, ::Test::I2>::_find_interface (base, id);
	}
};
#endif

// Tied implementation
#ifndef TEST_NO_TIED
template <class T>
class ServantTied <T, ::Test::I2>	: 
	public ImplementationTied <T, ::Test::I2, ::CORBA::Object>
{
public:
	ServantTied (T* tp, Boolean release) :
		ImplementationTied <T, ::Test::I2, ::CORBA::Object> (tp, release)
	{}
};
#endif

}
}

namespace POA_Test {

#ifndef TEST_NO_POA
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I2> I2;
#endif

#ifndef TEST_NO_TIED
template <class T>
class I2_tie :
	public ::CORBA::Nirvana::ServantTied <T, ::Test::I2>
{
public:
	I2_tie (T& t) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I2> (&t, FALSE)
	{}

	I2_tie (T& t, ::PortableServer::POA_ptr poa);	// Generate only if Object is derived

	I2_tie (T* tp, ::CORBA::Boolean release = TRUE) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I2> (tp, release)
	{}

	I2_tie (T* tp, ::PortableServer::POA_ptr poa, ::CORBA::Boolean release = TRUE);	// Generate only if Object is derived
};
#endif

}

#endif
