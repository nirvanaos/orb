// The Nirvana project.
// Object Request Broker.
// Standard Nirvana interface implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectAdapter.h"
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
class AbstractBaseImplNoRefCnt :
	public InterfaceImpl <S, AbstractBase>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.interface);
		return static_cast <S&> (*bridge);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, EnvironmentBridge* env)
	{
		try {
			_check_pointer (derived, Skeleton <S, Derived>::epv_.interface);
			return &static_cast <Bridge <Base>&> (*static_cast <S*> (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

template <class S>
class AbstractBaseImpl :
	public AbstractBaseImplNoRefCnt <S>,
	public RefCountBase
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			static_cast <S*> (itf)->_add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			static_cast <S*> (itf)->_remove_ref ();
	}
};

// Standard implementation of CORBA::Nirvana::ServantBase

class ServantBaseLinks
{
public:
	operator Bridge <Object>& ()
	{
		_activate ();
		return *servant_links_->object;
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
	ServantBaseLinks (Bridge <ServantBase>* servant, const Char* primary_interface) :
		servant_links_ (g_system->create_servant (servant, primary_interface))
	{}

	~ServantBaseLinks ()
	{
		g_system->destroy_servant (servant_links_);
	}

	void _activate ()
	{
		g_system->activate_object (servant_links_);
	}

protected:
	ServantLinks* servant_links_;
};

template <class S, class Primary>
class ServantBaseImpl :
	public AbstractBaseImpl <S>,
	public InterfaceImpl <S, ServantBase>,
	public ServantBaseLinks
{
public:
	T_ptr <Primary> _this ()
	{
		ServantBaseLinks::_activate ();
		return &static_cast <Bridge <Primary>&> (static_cast <S&> (*this));
	}

protected:
	ServantBaseImpl () :
		ServantBaseLinks (this, Primary::interface_id_)
	{}
};

}
}

#endif
