// Nirvana project
// Object Request Broker
// Static interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "Implementation.h"
#include "ServantStatic.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

//! Static implementation of ServantBase.
//! \tparam S Servant class.
template <class S>
class InterfaceStatic <S, ServantBase> :
	public InterfaceStaticBase <S, ServantBase>
{
public:
	operator Bridge <Object>& () const
	{
		return *Object_ptr (servant_base ());
	}

	// ServantBase operations

	static PortableServer::POA_ptr _default_POA ()
	{
		return servant_base ()->_default_POA ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return servant_base ()->_get_interface ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		return servant_base ()->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return false;
	}

protected:
	static Interface* _get_proxy ()
	{
		Interface* proxy = AbstractBase_ptr (servant_base ())->_query_interface (0);
		if (!proxy)
			::Nirvana::throw_MARSHAL ();
		return proxy;
	}

private:
	static ServantBase_ptr servant_base ()
	{
		return static_cast <ServantBase*> (export_struct_.core_object);
	}

	static __declspec (allocate(OLF_BIND)) const ::Nirvana::ExportObject export_struct_;
};

template <class S> __declspec (allocate(OLF_BIND))
const ::Nirvana::ExportObject InterfaceStatic <S, ServantBase>::export_struct_ { ::Nirvana::OLF_EXPORT_OBJECT, S::constant_name
, STATIC_BRIDGE (S, ServantBase) };

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, Object> :
	public InterfaceStaticBase <S, Object>
{
public:
	// Object operations

	static ImplementationDef_ptr _get_implementation ()
	{
		return object ()->_get_implementation ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return object ()->_get_interface ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		return object ()->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return object ()->_non_existent ();
	}

	static Boolean _is_equivalent (Object_ptr other_object)
	{
		return object ()->_is_equivalent (other_object);
	}

	static ULong _hash (ULong maximum)
	{
		return object ()->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

	static Interface* _get_proxy ()
	{
		Interface* proxy = AbstractBase_ptr (object ())->_query_interface (0);
		if (!proxy)
			::Nirvana::throw_MARSHAL ();
		return proxy;
	}

private:
	static Object_ptr object ()
	{
		return Object_ptr (static_cast <Object*> (export_struct_.core_object));
	}

	static __declspec (allocate(OLF_BIND)) const ::Nirvana::ExportLocal export_struct_;
};

template <class S> __declspec (allocate(OLF_BIND))
const ::Nirvana::ExportLocal InterfaceStatic <S, Object>::export_struct_{ ::Nirvana::OLF_EXPORT_LOCAL, S::constant_name
, STATIC_BRIDGE (S, AbstractBase) };

//! \class ImplementationStatic
//!
//! \brief A static implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.
//!               Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class ImplementationStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	Interface_ptr _query_interface (String_in id)
	{
		return FindInterface <Primary, Bases...>::find (*(S*)0, id);
	}

	static I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (
			std::conditional <std::is_base_of <InterfaceStatic <S, LocalObject>, ImplementationStatic <S, Primary, Bases...> >::value,
			InterfaceStatic <S, LocalObject>, InterfaceStatic <S, ServantBase>>::type::_get_proxy ());
	}
};

}
}

#endif
