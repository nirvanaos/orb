#ifndef NIRVANA_ORB_TEST_I3_S_H_
#define NIRVANA_ORB_TEST_I3_S_H_

#include "Test_I3_c.h"
#include "Server.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::Test::I3>
{
public:
	static const typename Bridge < ::Test::I3>::EPV epv_;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		Bridge <Interface>* itf;
		if (RepositoryId::compatible (Bridge < ::Test::I3>::interface_id_, id))
			itf = &static_cast <Bridge < ::Test::I3>&> (base);
		// Call all direct bases
		else if (!(itf = Skeleton <S, ::Test::I2>::_find_interface (base, id)))
			itf = Skeleton <S, ::Test::I1>::_find_interface (base, id);
		return itf;
	}

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
	public ServantBaseImpl <S, ::Test::I3>,
	public InterfaceImpl <S, ::Test::I3>,
	public InterfaceImpl <S, ::Test::I2>,
	public InterfaceImpl <S, ::Test::I1>
{
public:
	Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <S, ::Test::I3>::_find_interface (*this, id);
	}
};

// POA implementation
#ifndef TEST_NO_POA
template <>
class ServantPOA < ::Test::I3> :
	public virtual ServantPOA < ::Test::I1>,
	public virtual ServantPOA < ::Test::I2>,
	public InterfaceImpl <ServantPOA < ::Test::I3>, ::Test::I3>
{
public:
	virtual const Char* _primary_interface ()
	{
		return Bridge < ::Test::I3>::interface_id_;
	}

	virtual Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA < ::Test::I3>, ::Test::I3>::_find_interface (*this, id);
	}

	T_ptr < ::Test::I3> _this ()
	{
		ServantBaseImpl::_activate ();
		return this;
	}

	virtual Long op3 (Long p1) = 0;
};
#endif

// Static implementation
#ifndef TEST_NO_STATIC
template <class S>
class ServantStatic <S, ::Test::I3> :
	public ServantBaseStatic <S, ::Test::I3>,
	public InterfaceStatic <S, ::Test::I3>,
	public InterfaceStatic <S, ::Test::I2>,
	public InterfaceStatic <S, ::Test::I1>
{
public:
	static Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <S, ::Test::I3>::_find_interface (*(S*)nullptr, id);
	}
};
#endif

// Tied implementation
#ifndef TEST_NO_TIED
template <class T>
class ServantTied <T, ::Test::I3> :
	public ImplementationTied <T, ::Test::I3, ::Test::I2, ::Test::I1, ::CORBA::Object>
{
public:
	ServantTied (T* tp, Boolean release) :
		ImplementationTied <T, ::Test::I3, ::Test::I2, ::Test::I1, ::CORBA::Object> (tp, release)
	{}
};
#endif

}
}

namespace POA_Test {

#ifndef TEST_NO_POA
typedef ::CORBA::Nirvana::ServantPOA < ::Test::I3> I3;
#endif

#ifndef TEST_NO_TIED
template <class T>
class I3_tie :
	public ::CORBA::Nirvana::ServantTied <T, ::Test::I3>
{
public:
	I3_tie (T& t) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I3> (&t, FALSE)
	{}

	I3_tie (T& t, ::PortableServer::POA_ptr poa);	// Generate only if Object is derived

	I3_tie (T* tp, ::CORBA::Boolean release = TRUE) :
		::CORBA::Nirvana::ServantTied <T, ::Test::I3> (tp, release)
	{}

	I3_tie (T* tp, ::PortableServer::POA_ptr poa, ::CORBA::Boolean release = TRUE);	// Generate only if Object is derived
};
#endif

}

#endif
