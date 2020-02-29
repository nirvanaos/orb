#ifndef NIRVANA_ORB_SERVANTBASESTATIC_H_
#define NIRVANA_ORB_SERVANTBASESTATIC_H_

#include <Nirvana/Nirvana.h>
#include <Nirvana/OLF.h>
#include "AbstractBaseStatic.h"
#include "ReferenceCounterStatic.h"
#include "ServantBase_s.h"

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

	static Boolean _is_a (const String& type_id)
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
	static PortableServer::Servant servant_base ()
	{
		return static_cast <PortableServer::ServantBase*> (export_struct_.core_object);
	}

	static __declspec (allocate(OLF_BIND)) const ::Nirvana::ExportObject export_struct_;
};

template <class S> __declspec (allocate(OLF_BIND))
const ::Nirvana::ExportObject InterfaceStatic <S, PortableServer::ServantBase>::export_struct_{ ::Nirvana::OLF_EXPORT_OBJECT, S::constant_name
, STATIC_BRIDGE (S, PortableServer::ServantBase) };

}
}

#endif
