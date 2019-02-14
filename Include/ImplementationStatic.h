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

template <class S, class I> class ServantStaticAbstract;
template <class I> class ServantStatic;

// Static interface implementation

template <class S, class I>
class InterfaceStatic :
	public Skeleton <S, I>
{
	InterfaceStatic ();	// Never be instantiated
public:
	operator Bridge <I>& () const
	{
		return *_bridge ();
	}

	static Bridge <I>* _bridge ()
	{
		return reinterpret_cast <Bridge <I>*> (&bridge_);
	}

protected:
	static const typename Bridge <I>::EPV* bridge_;
};

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStatic<S, I>::bridge_ = &InterfaceStatic<S, I>::epv_;

// Static implementation of CORBA::AbstractBase

template <class S>
class AbstractBaseStatic :
	public InterfaceStatic <S, AbstractBase>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return *(S*)0;
	}

	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge*)
	{
		return itf;
	}

	template <class I>
	static void __release (Bridge <Interface>*)
	{}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, EnvironmentBridge* env)
	{
		try {
			return &static_cast <Bridge <Base>&> (*(S*)nullptr);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

// Static implementation of CORBA::Nirvana::ServantBase

template <class Primary> class StaticObjectImpl;

template <class Primary>
class StaticObject :
	public InterfaceStatic <typename StaticObjectImpl <Primary>::Implementation, ServantBase>
{
	StaticObject ();	// Never be instantiated
public:
	operator Bridge <Object>& () const
	{
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
	static const ServantLinks* const servant_links_;
	static const OLF_ObjectInfo object_info_;
};

template <class Primary>
const OLF_ObjectInfo StaticObject <Primary>::object_info_ = {reinterpret_cast <Bridge <ServantBase>*> (&StaticObject <Primary>::bridge_), Primary::object_id_};

template <class Primary>
class ServantBaseStatic :
	public AbstractBaseStatic <typename StaticObjectImpl <Primary>::Implementation>,
	public StaticObject <Primary>
{
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <typename StaticObjectImpl <Primary>::Implementation, Primary>::_bridge ();
	}
};

}
}

#endif
