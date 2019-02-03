// Nirvana project
// Object Request Broker
// Static interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

// Structures for OLF loader
struct OLF_ObjectInfo
{
	Bridge <ServantBase>* servant;
	const Char* primary_interface;
};

template <class S, class I> class ServantStatic;

// Static interface implementation

template <class S, class I>
class InterfaceStatic :
	public Skeleton <S, I>
{
	InterfaceStatic ();	// Never be instantiated
public:
	static Bridge <I>* _bridge ()
	{
		return (Bridge <I>*)&bridge_;
	}

private:
	static const typename Bridge <I>::EPV* bridge_;
};

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStatic<S, I>::bridge_ = &InterfaceStatic<S, I>::epv_;

// Static implementation of CORBA::AbstractBase

class LifeCycleStatic
{
public:
	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, EnvironmentBridge*)
	{
		return itf;
	}

	template <class I>
	static void _release (Bridge <Interface>*)
	{}
};

template <class S>
class AbstractBaseStatic :
	public InterfaceStatic <S, AbstractBase>,
	public LifeCycleStatic
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return *(S*)0;
	}

	template <class I, class B>
	static Bridge <I>& _narrow (B&)
	{
		return *InterfaceStatic <S, I>::_bridge ();
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>*, EnvironmentBridge*)
	{
		return InterfaceStatic <S, Base>::_bridge ();
	}
};

// Static implementation of CORBA::Object

template <class S>
class InterfaceStatic <S, Object> :
	public InterfaceStatic <S, ServantBase>
{
	InterfaceStatic ();	// Never be instantiated
public:
	static Bridge <Object>* _bridge ()
	{
		return servant_links_->object;
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

	static const ServantLinks* servant_links_;
};

template <class S, class Primary>
class ObjectStatic :
	public InterfaceStatic <S, Object>
{
	ObjectStatic ();	// Never be instantiated
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_bridge ();
	}

	static const OLF_ObjectInfo object_info_;
};

template <class S, class Primary>
const OLF_ObjectInfo ObjectStatic <S, Primary>::object_info_ = {(Bridge <ServantBase>*)&InterfaceStatic <S, Object>::servant_base_bridge_, Primary::object_id_};

}
}

#endif
