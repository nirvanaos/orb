// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectAdapter_c.h"
#include "POA_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "RefCountBase.h"

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

//! Standard interface implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Interface.
template <class S, class I>
class InterfaceImpl :
	public ServantBridge <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceImpl () :
		ServantBridge <I> (Skeleton <S, I>::epv_)
	{}
};

//! Dynamic object life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
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

//! Life cycle with reference counting.
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

//! Non copyable reference.
template <class S>
class LifeCycleNoCopy
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

//! Standard implementation of `CORBA::AbstractBase'.
 
template <class S, class Primary>
class AbstractBaseImpl :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, AbstractBase>
{
public:
	Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface <Primary>::find (static_cast <S&> (*this), id);
	}
};

//! Standard implementation of ServantBase.

class ServantBaseLinks :
	public ServantBridge <ServantBase>
{
public:
	operator Bridge <Object>& ()
	{
		_implicitly_activate ();
		return *servant_links_->object ();
	}

	operator ServantLinks_ptr () const
	{
		return servant_links_;
	}

	// ServantBase operations

	::PortableServer::POA_ptr _default_POA ()
	{
		return servant_links_->servant_base()->_default_POA ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return servant_links_->servant_base ()->_get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return servant_links_->servant_base ()->_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return servant_links_->servant_base ()->_non_existent ();
	}

protected:
	ServantBaseLinks (const EPV& epv) :
		ServantBridge <ServantBase> (epv),
		servant_links_ ((ServantLinks*)nullptr)
	{}
	
	ServantBaseLinks (const EPV& epv, const Char* primary_interface) :
		ServantBridge <ServantBase> (epv),
		servant_links_ ((ServantLinks*)nullptr)
	{
		_final_construct (primary_interface);
	}

	void _final_construct (const Char* primary_interface);

	void _implicitly_activate ();

	~ServantBaseLinks ()
	{
		release (servant_links_);
	}

protected:
	ServantLinks_ptr servant_links_;
};

//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
template <class S, class Primary>
class ServantBaseImpl :
	public AbstractBaseImpl <S, Primary>,
	public ServantBaseLinks,
	public Skeleton <S, ServantBase>
{
protected:
	ServantBaseImpl () :
		ServantBaseLinks (Skeleton <S, ServantBase>::epv_, Bridge <Primary>::interface_id_)
	{}

	T_ptr <Primary> _this ()
	{
		_implicitly_activate ();
		return static_cast <S*> (this);
	}
};

//! Standard implementation of `CORBA::LocalObject'.

class LocalObjectLinks :
	public ServantBridge <Object>
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

protected:
	LocalObjectLinks (const EPV& epv, const Char* primary_id);

	~LocalObjectLinks ()
	{
		release (object_);
	}

private:
	Object_ptr object_;
};

//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
template <class S, class Primary>
class LocalObjectImpl :
	public AbstractBaseImpl <S, Primary>,
	public LocalObjectLinks,
	public Skeleton <S, Object>
{
protected:
	LocalObjectImpl () :
		LocalObjectLinks (Skeleton <S, Object>::epv_, Bridge <Primary>::interface_id_)
	{}

	T_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (static_cast <Bridge <Primary>*> (static_cast <S*> (this)));
	}
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

//! \class	ImplementationPseudo
//!
//! \brief	An implementation of pseudo interface derived from AbstractBase.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationPseudo :
	public AbstractBaseImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
protected:
	ImplementationPseudo ()
	{}
};

//! \class	Implementation
//!
//! \brief	An implementation of interface derived from Object.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class Implementation :
	public ServantBaseImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
protected:
	Implementation ()
	{}
};

}
}

#endif
