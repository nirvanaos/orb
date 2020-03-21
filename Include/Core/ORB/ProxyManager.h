#ifndef NIRVANA_ORB_CORE_PROXYMANAGER_H_
#define NIRVANA_ORB_CORE_PROXYMANAGER_H_

#include <CORBA/Proxy/Proxy.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyManager
{
public:
	ProxyManager (Interface* obj_proxy, const Char* primary_iid, bool remote);

	struct InterfaceEntry
	{
		const Char* iid;
		size_t iid_len;
		Interface* proxy;
		DynamicServant_ptr deleter;
		CountedArray <Operation> operations;

		bool operator < (const InterfaceEntry& rhs) const
		{
			return RepositoryId::compare (iid, iid_len, rhs.iid, rhs.iid_len) < 0;
		}
	};

private:
	InterfaceEntry* interfaces_;
	ULong interface_cnt_;
};

}
}
}

#endif
