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
class AbstractBaseNoRefCnt :
	public InterfaceImpl <S, AbstractBase>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.interface);
		return static_cast <S&> (*bridge);
	}

	template <class I>
	static Bridge <I>& _narrow (Bridge <AbstractBase>& base)
	{
		S& servant = static_cast <S&> (base);
		servant._add_ref ();
		return static_cast <Bridge <I>&> (servant);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, EnvironmentBridge* env)
	{
		try {
			return &static_cast <Bridge <Base>&> (S::_implementation (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			S::_implementation (static_cast <Bridge <I>*> (itf))._add_ref ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return itf;
	}

	template <class I>
	static void _release (Bridge <Interface>* itf)
	{
		try {
			S::_implementation (static_cast <Bridge <I>*> (itf))._remove_ref ();
		} catch (...) {
		}
	}
};

template <class S>
class AbstractBaseImpl :
	public AbstractBaseNoRefCnt <S>,
	public RefCountBase
{};

// Standard implementation of CORBA::Object

class ServantBaseImpl
{
public:
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
	ServantBaseImpl (Bridge <ServantBase>* servant, const Char* primary_interface) :
		servant_links_ (g_system->create_servant (servant, primary_interface))
	{}

	~ServantBaseImpl ()
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

template <class S>
class InterfaceImpl <S, Object> :
	public InterfaceImpl <S, ServantBase>,
	public ServantBaseImpl
{
public:
	// For _narrow() and _wide() operations
	operator Bridge <Object>& ()
	{
		_activate ();
		return *servant_links_->object;
	}

protected:
	InterfaceImpl (const Char* primary_interface) :
		ServantBaseImpl (this, primary_interface)
	{}
};

template <class S, class Primary>
class ObjectImpl :
	public InterfaceImpl <S, Object>
{
public:
	T_ptr <Primary> _this ()
	{
		ServantBaseImpl::_activate ();
		return &static_cast <Bridge <Primary>&> (static_cast <S&> (*this));
	}

protected:
	ObjectImpl () :
		InterfaceImpl <S, Object> (Primary::interface_id_)
	{}
};

}
}

#endif
