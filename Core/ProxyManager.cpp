#include <Nirvana/Nirvana.h>
#include <Core/ORB/ProxyManager.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {
namespace Core {

using namespace ::Nirvana;
using namespace std;

ProxyManager::ProxyManager (Interface* obj_proxy, const Char* primary_iid, bool remote)
{
	ProxyFactory_var proxy_factory = g_binder->bind <ProxyFactory> (primary_iid);

	const InterfaceMetadata* metadata = proxy_factory->metadata ();
	ULong base_cnt = metadata->base_interfaces.size;
	interfaces_ = (InterfaceEntry*)g_memory->allocate (0, sizeof (InterfaceEntry) * (interface_cnt_ = base_cnt + 1), Memory::ZERO_INIT);
	interfaces_->iid = obj_proxy->_epv ().interface_id;
	interfaces_->iid_len = strlen (interfaces_->iid);
	interfaces_->proxy = obj_proxy;

	if (base_cnt) {
		try {
			const Char* const* base = metadata->base_interfaces.p;
			const Char* const* base_end = base + base_cnt;
			InterfaceEntry* ie = interfaces_;
			do {
				++ie;
				const Char* iid = *base;
				ie->iid = iid;
				ie->iid_len = strlen (iid);
			} while (base_end != ++base);

			sort (interfaces_, interfaces_ + interface_cnt_);
		} catch (...) {
				g_memory->release (interfaces_, sizeof (InterfaceEntry) * interface_cnt_);
				throw;
		}
	}
}

}
}
}
