#ifndef NIRVANA_ORB_PROXYMANAGER_C_H_
#define NIRVANA_ORB_PROXYMANAGER_C_H_

#include "Interface_c.h"

namespace CORBA {
namespace Nirvana {

class ProxyManager;
typedef T_ptr <ProxyManager> ProxyManager_ptr;
typedef T_var <ProxyManager> ProxyManager_var;
typedef T_out <ProxyManager> ProxyManager_out;

template <>
class Bridge <ProxyManager> :
	public BridgeMarshal <ProxyManager>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*add_ref) (Bridge <ProxyManager>*, EnvironmentBridge*);
			void (*remove_ref) (Bridge <ProxyManager>*);
			void (*enter) (Bridge <ProxyManager>*, EnvironmentBridge*);
			void (*leave) (Bridge <ProxyManager>*, EnvironmentBridge*);
			Bridge <Interface>* (*wide) (Bridge <ProxyManager>*, const Char*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <ProxyManager> (epv.interface)
	{}
};

}
}

#endif
