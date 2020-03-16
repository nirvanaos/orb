#ifndef NIRVANA_ORB_PROXY_H_
#define NIRVANA_ORB_PROXY_H_

#include "ProxyFactory_s.h"
#include "MarshalTraits.h"
#include "PlatformObjRef.h"
#include "PlatformRequest.h"
#include "PlatformProxyBase.h"
#include "TypeCodeInterface.h"
#include "TypeCodeString.h"
#include "TypeCodeSequence.h"
#include <Nirvana/stateless.h>

namespace CORBA {
namespace Nirvana {

template <class I> class ProxyFactoryImpl;

template <class I> class ProxyFactoryBase :
	public ServantStatic <ProxyFactoryImpl <I>, ProxyFactory>
{
public:
	Interface_ptr create_platform_proxy (
		PlatformObjRef_ptr proxy_manager, UShort interface_idx,
		DynamicServant_var& deleter)
	{
		PlatformProxy <I>* proxy =
			::Nirvana::stateless_create <PlatformProxy <I> > (proxy_manager, interface_idx);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}
};

}
}

#endif
