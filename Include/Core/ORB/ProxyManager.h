/// \file ProxyManager.h
#ifndef NIRVANA_ORB_CORE_PROXYMANAGER_H_
#define NIRVANA_ORB_CORE_PROXYMANAGER_H_

#include <CORBA/Proxy/Proxy.h>
#include <Core/Array.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// \brief Base for all proxies.
///        Implements AbstractBase and DII for proxies.
class ProxyManager
{
public:
	ProxyManager (IOReference_ptr ior, String_in primary_iid);

	struct InterfaceEntry
	{
		const Char* iid;
		size_t iid_len;
		Interface* proxy;
		DynamicServant_ptr deleter;
		CountedArray <Operation> operations;

		~InterfaceEntry () NIRVANA_NOEXCEPT
		{
			if (deleter)
				deleter->_delete ();
		}
	};

	struct OperationEntry
	{
		const Char* name;
		size_t name_len;
		OperationIndex idx;
	};

	InterfaceEntry* find_interface (String_in iid);
	OperationEntry* find_operation (String_in name);

private:
	struct IEPred;
	struct OEPred;

	void create_proxy (IOReference_ptr ior, InterfaceEntry& ie);

private:
	::Nirvana::Core::Array <InterfaceEntry> interfaces_;
	::Nirvana::Core::Array <OperationEntry> operations_;
};

}
}
}

#endif
