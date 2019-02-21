#ifndef IDL_TEST_I3_S_H_
#define IDL_TEST_I3_S_H_

#include "Test_I3_c.h"
#include "Test_I1_s.h"
#include "Test_I2_s.h"
#include "Server.h"

namespace CORBA {
namespace Nirvana {

template <>
class FindInterface < ::Test::I3>
{
public:
	template <class S>
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		Bridge <Interface>* itf;
		if (RepositoryId::compatible (Bridge < ::Test::I3>::interface_id_, id))
			itf = Interface::_duplicate (&static_cast <Bridge < ::Test::I3>&> (servant));
		// Call all direct bases
		else if (!(itf = FindInterface < ::Test::I2>::find (servant, id)))
			itf = FindInterface < ::Test::I1>::find (servant, id);
		return itf;
	}
};

template <class S>
class Skeleton <S, ::Test::I3>
{
public:
	static const typename Bridge < ::Test::I3>::EPV epv_;

protected:
	static Long _op3 (Bridge < ::Test::I3>* _b, Long p1, EnvironmentBridge* _env)
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
const Bridge < ::Test::I3>::EPV Skeleton <S, ::Test::I3>::epv_ = {
	{ // interface
		Bridge < ::Test::I3>::interface_id_,
		S::template __duplicate < ::Test::I3>,
		S::template __release < ::Test::I3>
	},
	{ // base
		S::template _wide <Object, ::Test::I3>,
		S::template _wide < ::Test::I1, ::Test::I3>,
		S::template _wide < ::Test::I2, ::Test::I3>
	},
	{ // epv
		S::_op3
	}
};

// Standard implementation

template <class S>
class Servant <S, ::Test::I3> :
	public ServantBaseImpl <S>,
	public InterfaceImpl <S, ::Test::I3>,
	public InterfaceImpl <S, ::Test::I2>,
	public InterfaceImpl <S, ::Test::I1>
{
public:
	Servant ()
	{
		ServantBaseImpl <S>::_final_construct (Bridge < ::Test::I3>::interface_id_);
	}

	T_ptr <::Test::I3> _this ()
	{
		ServantBaseLinks::_activate ();
		return this;
	}

	Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I3>::find (*this, id);
	}
};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA < ::Test::I3> :
	public virtual ServantPOA < ::Test::I1>,
	public virtual ServantPOA < ::Test::I2>,
	public InterfaceImpl <ServantPOA < ::Test::I3>, ::Test::I3>
{
public:
	ServantPOA ()
	{
		_final_construct (Bridge < ::Test::I3>::interface_id_);
	}

	T_ptr < ::Test::I3> _this ()
	{
		_activate ();
		return this;
	}

	virtual Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I3>::find (*this, id);
	}

	virtual Long op3 (Long p1) = 0;
};

}
}

namespace POA_Test {
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I3> I3;
}
#endif

// Static implementation
#ifndef TEST_NO_STATIC

namespace CORBA {
namespace Nirvana {

template <class S>
class ServantStatic <S, ::Test::I3> :
	public ServantBaseStatic <S, ::Test::I3>,
	public InterfaceStatic <S, ::Test::I3>,
	public InterfaceStatic <S, ::Test::I2>,
	public InterfaceStatic <S, ::Test::I1>
{
public:
	static Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I3>::find (*(ServantStatic < ::Test::I3>*)nullptr, id);
	}
};

}
}

#endif

// Tied implementation
#ifndef TEST_NO_TIED

namespace POA_Test {

template <class T> using I3_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I3>;

}

#endif

#endif
