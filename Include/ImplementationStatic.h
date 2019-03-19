// Nirvana project
// Object Request Broker
// Static interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "ObjectAdapter_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "DynamicServant_s.h"

namespace CORBA {
namespace Nirvana {

// Structures for OLF loader
struct OLF_ObjectInfo
{
	Bridge <PortableServer::ServantBase>* servant;
	Bridge <DynamicServant>* dynamic;
};

struct OLF_LocalObjectInfo
{
	Bridge <DynamicServant>* dynamic;
};

template <class S, class I> class ServantStatic;

//! Static interface traits
template <class S>
class ServantTraitsStatic
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return *(S*)0;
	}

	template <class I>
	static S& _servant (Bridge <I>* bridge)
	{
		return *(S*)0;
	}
};

template <class S>
class ServantTraitsStaticEx
{
public:
	template <class I>
	static S _implementation (Bridge <I>* bridge)
	{
		return S ();
	}

	template <class I>
	static S _servant (Bridge <I>* bridge)
	{
		return S ();
	}
};

template <class ... Bases>
class LifeCycleStatic : public Bases...
{
public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge*)
	{
		return itf;
	}

	template <class I>
	static void __release (Bridge <Interface>*)
	{}
};

template <class S, class I>
class InterfaceStaticBase :
	public Skeleton <S, I>
{
public:
	operator Bridge <I>& () const
	{
		return *_bridge ();
	}
	
	static T_ptr <I> _get_ptr ()
	{
		return static_cast <I*> (_bridge ());
	}

	static constexpr Bridge <I>* _bridge ()
	{
		return reinterpret_cast <Bridge <I>*> (&bridge_);
	}

protected:
	static const typename Bridge <I>::EPV* bridge_;
};

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStaticBase <S, I>::bridge_ = &InterfaceStaticBase <S, I>::epv_;

template <class S, class I>
class InterfaceStatic :
	public InterfaceStaticBase <S, I>
{};

//! Static implementation of PortableServer::ServantBase.
//! \tparam S Servant class.
template <class S>
class InterfaceStatic <S, PortableServer::ServantBase> :
	public InterfaceStaticBase <S, PortableServer::ServantBase>,
	public InterfaceStaticBase <S, DynamicServant>
{
public:
	operator Bridge <Object>& () const
	{
		return *Object_ptr (servant_base_);
	}

	// ServantBase operations

	static PortableServer::POA_ptr _default_POA ()
	{
		return servant_base_->_default_POA ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return servant_base_->_get_interface ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		return servant_base_->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return false;
	}

	static const OLF_ObjectInfo object_info_;
	static PortableServer::Servant servant_base_;
};

template <class S>
const OLF_ObjectInfo InterfaceStatic <S, PortableServer::ServantBase>::object_info_ = {InterfaceStaticBase <S, PortableServer::ServantBase>::_bridge (), InterfaceStaticBase <S, DynamicServant>::_bridge ()};

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStaticBase <S, DynamicServant>,
	public InterfaceStaticBase <S, Object>
{
public:
	// Object operations

	static ImplementationDef_ptr _get_implementation ()
	{
		return object_->_get_implementation ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return object_->_get_interface ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		return object_->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return object_->_non_existent ();
	}

	static Boolean _is_equivalent (Object_ptr other_object)
	{
		return object_->_is_equivalent (other_object);
	}

	static ULong _hash (ULong maximum)
	{
		return object_->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

	static const OLF_LocalObjectInfo object_info_;
	static Object_ptr object_;
};

template <class S>
const OLF_LocalObjectInfo InterfaceStatic <S, LocalObject>::object_info_ = {InterfaceStatic <S, DynamicServant>::_bridge ()};

template <class S, class I>
class ImplementationStaticSingle :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic <>,
	public InterfaceStatic <S, I>
{
public:
	static T_ptr <I> _this ()
	{
		return InterfaceStatic <S, I>::_get_ptr ();
	}
};

//! \class	ImplementationStatic
//!
//! \brief	A static implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.
//! 								Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class ImplementationStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic <>,
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::interface_id_;
	}

	Interface_ptr _query_interface (const Char* id)
	{
		return Interface::_duplicate (FindInterface <Primary, Bases...>::find (*(S*)nullptr, id));
	}

	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}
};

}
}

#endif
