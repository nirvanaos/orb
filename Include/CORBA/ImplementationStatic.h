// Nirvana project
// Object Request Broker
// Static interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "Implementation.h"
#include "ServantStatic.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class InterfaceStatic <S, ReferenceCounter> :
	public InterfaceStaticBase <S, ReferenceCounter>
{
public:
	static void __add_ref (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{}

	static void __remove_ref (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{}

	static ULong __refcount_value (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{
		return 1;
	}
};

//! Static implementation of PortableServer::ServantBase.
//! \tparam S Servant class.
template <class S>
class InterfaceStatic <S, PortableServer::ServantBase> :
	public InterfaceStaticBase <S, PortableServer::ServantBase>,
	public InterfaceStatic <S, ReferenceCounter>
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

private:
	static PortableServer::Servant servant_base ()
	{
		return servant_base_;
	}

	static Bridge <PortableServer::ServantBase>*& servant_base_;
};

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStaticBase <S, LocalObject>,
	public InterfaceStatic <S, Object>,
	public InterfaceStatic <S, ReferenceCounter>
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

private:
	static Object_ptr object ()
	{
		return object_;
	}

	static Bridge <Object>*& object_;
};

template <class S, class Primary, class ... Bases>
class ImplementationStaticPseudo :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic <>,
	public InterfaceStatic <S, Bases> ...,
	public InterfaceStatic <S, Primary>
{
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
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
	Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <Primary, Bases...>::find (*(S*)nullptr, id);
	}

	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}
};

}
}

#endif
