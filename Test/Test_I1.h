#pragma once
#include "PortableServer.h"

namespace Test {

class I1;
typedef CORBA::Nirvana::T_ptr <I1> I1_ptr;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <Test::I1> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge <Test::I1>*, Environment*);
		}
		base;

		struct
		{
			Long (*op1) (Bridge <Test::I1>*, Long p1, Environment*);
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
class Client <T, Test::I1> :
	public ClientBase <T, Test::I1>
{
public:
	Long op1 (Long p1);
};

template <class T>
Long Client <T, Test::I1>::op1 (Long p1)
{
	Environment _env;
	Bridge <Test::I1>& _b = _bridge ();
	Long _ret = (_b._epv ().epv.op1) (&_b, p1, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Test {

class I1 :
	public CORBA::Nirvana::ClientInterface <I1>,
	public CORBA::Nirvana::Client <I1, CORBA::Object>
{
public:
	typedef I1_ptr _ptr_type;

	operator CORBA::Object& ()
	{
		CORBA::Environment _env;
		CORBA::Object* _ret = static_cast <CORBA::Object*> ((_epv ().base.CORBA_Object) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

}

namespace PortableServer {
namespace Nirvana {

template <class S>
class Skeleton <S, Test::I1>
{
public:
	static const typename Bridge <Test::I1>::EPV sm_epv;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Test::I1>::_primary_interface (), id))
			return &S::_narrow <Test::I1> (base);
		else
			return Skeleton <S, Object>::_find_interface (base, id);
	}

protected:
	static Long _op1 (Bridge <Test::I1>* _b, Long p1, Environment* _env)
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
};

template <class S>
const Bridge <Test::I1>::EPV Skeleton <S, Test::I1>::sm_epv = {
	{ // interface
		S::_duplicate <Test::I1>,
		S::_release <Test::I1>
	},
	{ // base
		S::_wide <Object, Test::I1>
	},
	{ // epv
		S::_op1
	}
};

// Standard implementation

template <class S>
class Servant <S, Test::I1> :
	public Implementation <S, Test::I1, BaseImpl <S, Object>>
{};

// POA implementation

template <>
class ServantPOA <Test::I1> :
	public Implementation <ServantPOA <Test::I1>, Test::I1, BaseImplPOA <>>
{
public:
	virtual Long op1 (Long p1) = 0;

	virtual Boolean _is_a (const Char* type_id)
	{
		return Bridge <Test::I1>::___is_a (type_id);
	}

protected:
	virtual Bridge <Interface>* _POA_find_interface (const Char* id)
	{
		return Skeleton <ServantPOA <Test::I1>, Test::I1>::_find_interface (*this, id);
	}
};

}
}

namespace Test {

typedef PortableServer::Nirvana::ServantPOA <I1> POA_I1;

}
