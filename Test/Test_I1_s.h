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
			return Interface::_duplicate(&static_cast <Bridge < ::Test::I1>&> (servant));
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
		Bridge < ::Test::I1>::interface_id_,
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
	public ServantBaseImpl <S>,
	public InterfaceImpl <S, ::Test::I1>
{
public:
	Servant ()
	{
		ServantBaseImpl <S>::_final_construct (Bridge < ::Test::I1>::interface_id_);
	}

	T_ptr <::Test::I1> _this ()
	{
		ServantBaseLinks::_activate ();
		return this;
	}

	Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I1>::find (static_cast <S&> (*this), id);
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
	public virtual ServantPOA <ServantBase>,
	public InterfaceImpl <ServantPOA < ::Test::I1>, ::Test::I1>
{
public:
	ServantPOA ()
	{
		_final_construct (Bridge < ::Test::I1>::interface_id_);
	}

	virtual Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I1>::find (*this, id);
	}

	T_ptr < ::Test::I1> _this ()
	{
		_activate ();
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

namespace POA_Test {

template <class T> using I1_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I1>;

}

#endif

#endif
