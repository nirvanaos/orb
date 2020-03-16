#ifndef NIRVANA_ORB_PROXY_H_
#define NIRVANA_ORB_PROXY_H_

#include "../CORBA.h"
#include "../FindInterface.h"
#include "ProxyFactory_s.h"
#include "MarshalTraits.h"
#include "PlatformObjRef.h"
#include "PlatformRequest.h"
#include "PlatformProxyBase.h"
#include "TypeCodeString.h"
#include "TypeCodeSequence.h"
#include "TypeCodeException.h"
#include <Nirvana/stateless.h>

namespace CORBA {
namespace Nirvana {

template <class I> class ProxyFactoryImpl;

/// Proxy factory implements ProxyFactory and TypeCode interfaces.
template <class I> class ProxyFactoryBase :
	public InterfaceStaticBase <ProxyFactoryImpl <I>, AbstractBase>,
	public TypeCodeWithId <ProxyFactoryImpl <I>, tk_objref, I::interface_id_>,
	public TypeCodeOps <I_var <I> >,
	public InterfaceStaticBase <ProxyFactoryImpl <I>, ProxyFactory>
{
public:
	// AbstractBase
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <ProxyFactory, TypeCode>::find (*(ProxyFactoryImpl <I>*)0, id);
	}

	// ProxyFactory
	Interface_ptr create_platform_proxy (
		PlatformObjRef_ptr proxy_manager, UShort interface_idx,
		DynamicServant_var& deleter)
	{
		PlatformProxy <I>* proxy =
			::Nirvana::stateless_create <PlatformProxy <I> > (proxy_manager, interface_idx);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

	// TypeCode

	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return name_;
	}

	static const Char name_ [];
};

}
}

#endif
