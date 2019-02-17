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

template <class S, class I> class ServantStatic;

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

	static constexpr Bridge <I>* _bridge ()
	{
		return reinterpret_cast <Bridge <I>*> (&bridge_);
	}

protected:
	static const typename Bridge <I>::EPV* bridge_;
};

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStatic <S, I>::bridge_ = &InterfaceStatic <S, I>::epv_;

// Static implementation of CORBA::AbstractBase

class LifeCycleStatic
{
public:
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf)
	{
		return itf;
	}

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

	template <class I>
	static S& _servant (Bridge <I>* bridge)
	{
		return *(S*)0;
	}
};

// Static implementation of CORBA::Nirvana::ServantBase

template <class S, class Primary>
class StaticObject :
	public InterfaceStatic <S, ServantBase>
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

	static const OLF_ObjectInfo object_info_;
	static const ServantLinks* servant_links_;
};

template <class S, class Primary>
const OLF_ObjectInfo StaticObject <S, Primary>::object_info_ = {StaticObject <S, Primary>::_bridge (), Bridge <Primary>::interface_id_};

template <class S, class Primary>
class ServantBaseStatic :
	public AbstractBaseStatic <S>,
	public StaticObject <S, Primary>
{
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_bridge ();
	}
};

}
}

#endif
