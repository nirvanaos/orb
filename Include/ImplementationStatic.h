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

template <class S, class I>
class InterfaceStatic :
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
const typename Bridge <I>::EPV* InterfaceStatic <S, I>::bridge_ = &InterfaceStatic <S, I>::epv_;

class LifeCycleStatic
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

//! Static implementation of CORBA::AbstractBase
template <class S, class Primary>
class AbstractBaseStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public InterfaceStatic <S, AbstractBase>
{
public:
	static Interface_ptr _query_interface (const Char* id)
	{
		return Skeleton <S, Primary>::_find_interface (*(S*)nullptr, id);
	}
};

//! Static implementation of ServantBase
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S, class Primary>
class ServantBaseStatic :
	public AbstractBaseStatic <S, Primary>,
	public InterfaceStatic <S, ServantBase>
{
public:
	operator Bridge <Object>& () const
	{
		return *ServantLinks_ptr (servant_links_)->object ();
	}

	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}

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

template <class S, class Primary>
const OLF_ObjectInfo ServantBaseStatic <S, Primary>::object_info_ = {InterfaceStatic <S, ServantBase>::_bridge (), Bridge <Primary>::interface_id_};

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S, class Primary>
class LocalObjectStatic :
	public AbstractBaseStatic <S, Primary>,
	public InterfaceStatic <S, Object>
{
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}

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

template <class S, class Primary>
const OLF_LocalObjectInfo LocalObjectStatic <S, Primary>::object_info_ = {InterfaceStatic <S, AbstractBase>::_bridge (), Bridge <Primary>::interface_id_};

template <class S, class I>
class ImplementationSingleStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public InterfaceStatic <S, I>
{};

template <class S, class Primary, class ... Bases>
class ImplementationPseudoStatic :
	public AbstractBaseStatic <S, Primary>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{};

template <class S, class Primary, class ... Bases>
class ImplementationStatic :
	public ServantBaseStatic <S, Primary>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{};

}
}

#endif
