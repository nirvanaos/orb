#ifndef IDL_TEST_I2_S_H_
#define IDL_TEST_I2_S_H_

#include "Test_I2_c.h"
#include "Server.h"

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
			return &static_cast <Bridge < ::Test::I2>&> (base);
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
		S::template __duplicate < ::Test::I2>,
		S::template __release < ::Test::I2>
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
	public ServantBaseImpl <S, ::Test::I2>,
	public InterfaceImpl <S, ::Test::I2>
{
public:
	Interface_ptr _find_interface (const Char* id)
	{
		return Skeleton <S, ::Test::I2>::_find_interface (*this, id);
	}
};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

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

	virtual Interface_ptr _find_interface (const Char* id)
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

}
}

namespace POA_Test {
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I2> I2;
}
#endif

// Static implementation
#ifndef TEST_NO_STATIC
namespace POA_Test {
class I2_static;
}

namespace CORBA {
namespace Nirvana {

template <>
class StaticObjectImpl <::Test::I2>
{
public:
	typedef POA_Test::I2_static Implementation;
};

template <class S>
class ServantStatic <S, ::Test::I2> :
	public ServantBaseStatic <S, ::Test::I2>,
	public InterfaceStatic <S, ::Test::I2>
{
public:
	static Interface_ptr _find_interface (const Char* id)
	{
		return Skeleton <S, ::Test::I2>::_find_interface (*(S*)nullptr, id);
	}
};

}
}

#endif

// Tied implementation
#ifndef TEST_NO_TIED
namespace CORBA {
namespace Nirvana {

template <class T>
class ServantTied <T, ::Test::I2> :
	public ImplementationTied <T, ::Test::I2, ::CORBA::Object>
{
public:
	ServantTied (T* tp, Boolean release) :
		ImplementationTied <T, ::Test::I2, ::CORBA::Object> (tp, release)
	{}
};

}
}

namespace POA_Test {
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
}
#endif

#endif
