// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectAdapter_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "RefCountBase.h"

namespace CORBA {
namespace Nirvana {

/// Standard servant mix-in.
/// \tparam S Servant class implementing operations. Must derive from this mix-in.
/// \tparam I Primary interface.
template <class S, class I> class Servant;

/// Standard (dynamic) servant implementation.
/// \tparam S Servant class.
template <class S>
class Implementation
{
public:
	template <class I>
	static S& _servant (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.interface);
		return static_cast <S&> (*bridge);
	}

	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return _servant (bridge);
	}
};

/// Standard interface implementation
/// \tparam S Servant class implementing operations. Must derive from this mix-in.
/// \tparam I Interface.
template <class S, class I>
class InterfaceImpl :
	public Bridge <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceImpl () :
		Bridge <I> (Skeleton <S, I>::epv_)
	{}
};

/// Dynamic object life cycle.
/// \tparam S Class implementing _duplicate() and _release() operations.
template <class S>
class LifeCycleDynamic
{
public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			return S::_duplicate (static_cast <Bridge <I>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			S::_release (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

/// Life cycle with reference counting.
template <class S>
class LifeCycleRefCnt :
	public LifeCycleDynamic <S>,
	public RefCountBase
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			S::_servant (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			S::_servant (itf)._remove_ref ();
	}
};

/// Non copyable reference.
template <class S>
class LifeCycleNoCopy
{
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		env->set_exception (NO_IMPLEMENT ());
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			delete &S::_servant (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

/// Standard implementation of CORBA::AbstractBase
template <class S>
class AbstractBaseImpl :
	public Implementation <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, AbstractBase>
{};

// Standard implementation of CORBA::Nirvana::ServantBase

class ServantBaseLinks :
	public Bridge <ServantBase>
{
public:
	operator Bridge <Object>& ()
	{
		_activate ();
		return *servant_links_->object;
	}

	operator const ServantLinks& () const
	{
		return *servant_links_;
	}

	// ServantBase operations

	POA_ptr _default_POA ()
	{
		return ServantBase_ptr (servant_links_->servant_base)->_default_POA ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return ServantBase_ptr (servant_links_->servant_base)->_get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return ServantBase_ptr (servant_links_->servant_base)->_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return ServantBase_ptr (servant_links_->servant_base)->_non_existent ();
	}

protected:
	ServantBaseLinks (const EPV& epv) :
		Bridge <ServantBase> (epv),
		servant_links_ (nullptr)
	{}

	void _final_construct (const Char* primary_interface)
	{
		servant_links_ = g_system->create_servant (this, primary_interface);
	}

	~ServantBaseLinks ()
	{
		g_system->destroy_servant (servant_links_);
	}

	void _activate ()
	{
		::PortableServer::POA_var poa = ServantBase_ptr (this)->_default_POA ();
		poa->activate_object (*this);
	}

protected:
	ServantLinks* servant_links_;
};

/// Standard implementation of CORBA::Nirvana::ServantBase
/// \tparam S Servant class implementing operations.
template <class S>
class ServantBaseImpl :
	public AbstractBaseImpl <S>,
	public ServantBaseLinks,
	public Skeleton <S, ServantBase>
{
protected:
	ServantBaseImpl () :
		ServantBaseLinks (Skeleton <S, ServantBase>::epv_)
	{}
};

}
}

#endif
