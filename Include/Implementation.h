// The Nirvana project.
// Object Request Broker.
// Standard Nirvana interface implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectAdapter_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "RefCountBase.h"

namespace CORBA {
namespace Nirvana {

template <class S, class I> class Servant;

// Standard single interface implementation

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

// Standard implementation of CORBA::AbstractBase

template <class S>
class AbstractBaseNoLifeCycle :
	public InterfaceImpl <S, AbstractBase>
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

template <class Base>
class LifeCycleRefCnt :
	public Base,
	public RefCountBase
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			Base::ServantType::_servant (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			Base::ServantType::_servant (itf)._remove_ref ();
	}
};

template <class S>
class AbstractBaseImpl :
	public LifeCycleRefCnt <AbstractBaseNoLifeCycle <S> >
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
