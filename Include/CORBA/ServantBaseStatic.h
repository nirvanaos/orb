#ifndef NIRVANA_ORB_SERVANTBASESTATIC_H_
#define NIRVANA_ORB_SERVANTBASESTATIC_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "AbstractBaseStatic.h"
#include "ReferenceCounterStatic.h"
#include "ServantBase_s.h"
#include "servant_core.h"

namespace CORBA {
namespace Nirvana {

//! Static implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceStatic <S, PortableServer::ServantBase> :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStatic <S, ReferenceCounter>,
	public InterfaceStaticBase <S, PortableServer::ServantBase>
{
public:
	static Bridge <Object>* _get_object (String_in iid)
	{
		return get_object_from_core (servant_base (), iid);
	}

	// ServantBase operations

	static PortableServer::POA_var _default_POA ()
	{
		return servant_base ()->_default_POA ();
	}

	static InterfaceDef_var _get_interface ()
	{
		return servant_base ()->_get_interface ();
	}

	static Boolean _is_a (const String& type_id)
	{
		return servant_base ()->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return false;
	}

	static PortableServer::Servant __core_servant ()
	{
		return servant_base ();
	}

protected:
	static Interface* _get_proxy ()
	{
		return get_proxy (servant_base ());
	}

private:
	static PortableServer::Servant servant_base ()
	{
		return static_cast <PortableServer::ServantBase*> (export_struct_.core_object);
	}

	static NIRVANA_OLF_SECTION const ::Nirvana::ExportObject export_struct_;
};

template <class S> NIRVANA_OLF_SECTION
const ::Nirvana::ExportObject InterfaceStatic <S, PortableServer::ServantBase>::export_struct_{ ::Nirvana::OLF_EXPORT_OBJECT, S::constant_name
, STATIC_BRIDGE (PortableServer::ServantBase, S) };

}
}

#endif
