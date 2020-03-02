#include "ProxyManager.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

ProxyManager::ProxyManager (const Bridge <Object>::EPV& proxy_impl, AbstractBase_ptr servant) :
	Bridge <Object> (proxy_impl),
	servant_ (servant),
	ref_cnt_ (0),
	sync_domain_ (::Nirvana::SyncDomainTraits::_duplicate (::Nirvana::g_current->sync_domain_traits ()))
{}

Interface_ptr ProxyManager::_query_interface (const String& iid)
{
	if (!iid.empty ()) {
		switch (RepositoryId::check (Object::interface_id_, iid)) {
			case RepositoryId::COMPATIBLE:
				return this;
			case RepositoryId::INCOMPATIBLE_VERSION:
				return 0;
		}
	}

	// Real implementation must return proxy
	// For test we just shortcut to servant
	return servant_->_query_interface (iid);
}

}
}
}
