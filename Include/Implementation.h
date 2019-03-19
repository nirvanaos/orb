// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectAdapter_c.h"
#include "POA_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "DynamicServant_s.h"
#include "LocalObject_c.h"
#include "RefCountBase.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

//! Standard servant mix-in.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Primary interface.
template <class S, class I> class Servant;

//! Standard (dynamic) servant traits.
//! \tparam S Servant class, derived from this.
template <class S>
class ServantTraits
{
public:
	template <class I>

	//! \fn	static S& ServantTraits::_servant (Bridge <I>* bridge)
	//!
	//! \brief	Returns the servant from the given bridge pointer.
	//!
	//! \param [in]	bridge	The bridge pointer.
	//!
	//! \return	A reference to the servant.

	static S& _servant (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.interface);
		return static_cast <S&> (*bridge);
	}

	template <class I>

	//! \fn	static S& ServantTraits::_implementation (Bridge <I>* bridge)
	//!
	//! \brief	Returns the implementation from the given bridge pointer.
	//!
	//! \param [in]	bridge	The bridge pointer.
	//!
	//! \return	A reference to the implementation.

	static S& _implementation (Bridge <I>* bridge)
	{
		return _servant (bridge);
	}
};

//! Dynamic object life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
template <class S, class ... Bases>
class LifeCycleDynamic : public Bases...
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

//! Life cycle with reference counting.
template <class S, class ... Bases>
class LifeCycleRefCnt :
	public LifeCycleDynamic <S, Bases...>,
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

//! Non copyable reference.
template <class S, class ... Bases>
class LifeCycleNoCopy : public Bases...
{
public:
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

//! Standard interface implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Interface.
template <class S, class I>
class InterfaceImplBase :
	public Bridge <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceImplBase () :
		Bridge <I> (Skeleton <S, I>::epv_)
	{}
};

template <class S, class I>
class InterfaceImpl :
	public InterfaceImplBase <S, I>
{};

//! \brief Implements delegate to the core ServantBase implementation.
class ServantBaseLink :
	public Bridge <PortableServer::ServantBase>
{
public:
	operator Bridge <Object>& ()
	{
		return *Object_ptr (servant_base_);
	}

	// ServantBase operations

	PortableServer::POA_ptr _default_POA () const
	{
		return servant_base_->_default_POA ();
	}

	InterfaceDef_ptr _get_interface () const
	{
		return servant_base_->_get_interface ();
	}

	Boolean _is_a (const Char* type_id) const
	{
		return servant_base_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return false;
	}

	// Our extension
	Boolean _is_active () const
	{
		return !servant_base_->_non_existent ();
	}

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& servant_base, Bridge <DynamicServant>& dynamic_servant);

	void _implicitly_activate ();

	~ServantBaseLink ()
	{
		release (servant_base_);
	}

protected:
	PortableServer::Servant servant_base_;
};

//! Standard implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, PortableServer::ServantBase> :
	public Skeleton <S, PortableServer::ServantBase>,
	public InterfaceImplBase <S, DynamicServant>,
	public ServantBaseLink
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_, *this)
	{}
};

//! Standard implementation of `CORBA::LocalObject'.

class LocalObjectLink
{
public:
	// Object operations

	ImplementationDef_ptr _get_implementation ()
	{
		return object_->_get_implementation ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return object_->_get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return object_->_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return object_->_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		return object_->_is_equivalent (other_object);
	}

	ULong _hash (ULong maximum)
	{
		return object_->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	LocalObjectLink (DynamicServant_ptr servant);

	~LocalObjectLink ()
	{
		release (object_);
	}

private:
	Object_ptr object_;
};

//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public InterfaceImplBase <S, DynamicServant>,
	public InterfaceImplBase <S, Object>,
	public LocalObjectLink
{
protected:
	InterfaceImpl () :
		LocalObjectLink (this)
	{}
};

//! \class	ImplementationSingle
//!
//! \brief	An implementation of single pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	I	interface.

template <class S, class I>
class ImplementationSingle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, I>
{
protected:
	ImplementationSingle ()
	{}
};

//! \class	Implementation
//!
//! \brief	An implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.
//! 								Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class Implementation :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, AbstractBase>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
	class DummyActivator
	{
	public:
		static void _implicitly_activate ()
		{}
	};

public:
	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::interface_id_;
	}

	Interface_ptr _query_interface (const Char* id)
	{
		return Interface::_duplicate (FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id));
	}

	T_ptr <Primary> _this ()
	{
		std::conditional < std::is_base_of <ServantBaseLink, Implementation <S, Primary, Bases...> >::value, 
			ServantBaseLink, DummyActivator>::type::_implicitly_activate ();
		return this;
	}

protected:
	Implementation ()
	{}
};

}
}

#endif
