#ifndef IDL_TEST_I2_S_H_
#define IDL_TEST_I2_S_H_

#include "Test_I2_c.h"
#include "Server.h"

namespace CORBA {
namespace Nirvana {

template <>
class FindInterface < ::Test::I2>
{
public:
	template <class S>
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		if (RepositoryId::compatible (Bridge < ::Test::I2>::interface_id_, id))
			return Interface::_duplicate (&static_cast <Bridge < ::Test::I2>&> (servant));
		else
			return FindInterface <Object>::find (servant, id);
	}
};

template <class S>
class Skeleton <S, ::Test::I2>
{
public:
	static const typename Bridge < ::Test::I2>::EPV epv_;

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
		Bridge < ::Test::I2>::interface_id_,
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
	public Implementation <S, ::Test::I2>
{};

}
}

// POA implementation
#ifndef TEST_NO_POA
namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA < ::Test::I2> :
	public ImplementationPOA < ::Test::I2>
{
public:
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

namespace CORBA {
namespace Nirvana {

template <class S>
class ServantStatic <S, ::Test::I2> :
	public ServantBaseStatic <S, ::Test::I2>,
	public InterfaceStatic <S, ::Test::I2>
{
public:
	static Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface < ::Test::I2>::find (*(S*)nullptr, id);
	}
};

}
}

#endif

// Tied implementation
#ifndef TEST_NO_TIED

namespace POA_Test {

template <class T> using I2_tie = ::CORBA::Nirvana::ServantTied <T, ::Test::I2>;

}

#endif

#endif
