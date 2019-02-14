#ifndef IDL_TEST_I1_S_H_
#define IDL_TEST_I1_S_H_

#include "Test_I1_c.h"
#include "Server.h"

namespace CORBA {
namespace Nirvana {

template <>
class FindInterface < ::Test::I1>
{
public:
	template <class S>
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		if (RepositoryId::compatible (Bridge < ::Test::I1>::interface_id_, id))
			return &static_cast <Bridge < ::Test::I1>&> (servant);
		else
			return FindInterface <Object>::find (servant, id);
	}
};

template <class S>
class Skeleton <S, ::Test::I1>
{
public:
	static const typename Bridge < ::Test::I1>::EPV epv_;

protected:
	static Long _op1 (Bridge < ::Test::I1>* _b, Long p1, EnvironmentBridge* _env)
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

	static void _throw_NO_IMPLEMENT (Bridge < ::Test::I1>* _b, EnvironmentBridge* _env)
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
const Bridge < ::Test::I1>::EPV Skeleton <S, ::Test::I1>::epv_ = {
	{ // interface
		S::template __duplicate < ::Test::I1>,
		S::template __release < ::Test::I1>
	},
	{ // base
		S::template _wide <Object, ::Test::I1>
	},
	{ // epv
		S::_op1,
		S::_throw_NO_IMPLEMENT
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I1> :
	public ServantBaseImpl <S, ::Test::I1>,
	public InterfaceImpl <S, ::Test::I1>
{
public:
	Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I1>::find (*this, id);
	}
};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA < ::Test::I1> :
	public virtual ObjectPOA,
	public InterfaceImpl <ServantPOA < ::Test::I1>, ::Test::I1>
{
public:
	virtual const Char* _primary_interface ()
	{
		return Bridge < ::Test::I1>::interface_id_;
	}

	virtual Interface_ptr _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA < ::Test::I1>, ::Test::I1>::_find_interface (*this, id);
	}

	T_ptr < ::Test::I1> _this ()
	{
		ServantBaseImpl::_activate ();
		return this;
	}

	virtual Long op1 (Long p1) = 0;
	virtual void throw_NO_IMPLEMENT () = 0;
};

}
}

namespace POA_Test {
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I1> I1;
}

#endif

// Static implementation
#ifndef TEST_NO_STATIC

namespace CORBA {
namespace Nirvana {

template <class S>
class ServantStatic <S, ::Test::I1> :
	public ServantBaseStatic <S, ::Test::I1>,
	public InterfaceStatic <S, ::Test::I1>
{
public:
	static Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I1>::find (*(S*)nullptr, id);
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
class ServantTied <T, ::Test::I1> :
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

#endif
