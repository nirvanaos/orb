// Nirvana project
// Object Request Broker
// Static interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "ObjectAdapter_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"

namespace CORBA {
namespace Nirvana {

// Structures for OLF loader
struct OLF_ObjectInfo
{
	Bridge <ServantBase>* servant;
	const Char* primary_interface;
};

struct OLF_LocalObjectInfo
{
	Bridge <AbstractBase>* servant;
	const Char* primary_interface;
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

//! Static implementation of ServantBase
//! \tparam S Servant class.
template <class S>
class InterfaceStatic <S, ServantBase> :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStaticBase <S, ServantBase>
{
public:
	// ServantBase operations

	static ::PortableServer::POA_ptr _default_POA ()
	{
		return servant_links_->servant_base ()->_default_POA ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return servant_links_->servant_base ()->_get_interface ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		return servant_links_->servant_base ()->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return servant_links_->servant_base ()->_non_existent ();
	}

	static const OLF_ObjectInfo object_info_;
	static ServantLinks_ptr servant_links_;
};

template <class S>
const OLF_ObjectInfo InterfaceStatic <S, ServantBase>::object_info_ = {InterfaceStaticBase <S, ServantBase>::_bridge (), S::primary_interface_};

template <class S>
class InterfaceStatic <S, Object> :
	public InterfaceStatic <S, ServantBase>
{
public:
	operator Bridge <Object>& () const
	{
		return *ServantLinks_ptr (InterfaceStatic <S, ServantBase>::servant_links_)->object ();
	}
};

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStatic <S, AbstractBase>,
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

	static const OLF_LocalObjectInfo object_info_;
	static Object_ptr object_;
};

template <class S>
const OLF_LocalObjectInfo InterfaceStatic <S, LocalObject>::object_info_ = {InterfaceStatic <S, AbstractBase>::_bridge (), S::primary_interface_};

template <class S, class Primary, class ... Bases>
class ImplementationStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic <>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>,
	public PrimaryInterface <Primary>
{
public:
	Interface_ptr _query_interface (const Char* id)
	{
		return Interface::_duplicate (InterfaceFinder <S, Primary, Bases...>::find (*(S*)nullptr, id));
	}

	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}

};

}
}

#endif
