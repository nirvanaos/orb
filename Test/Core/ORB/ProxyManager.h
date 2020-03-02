#ifndef NIRVANA_ORB_CORE_PROXYMANAGER_H_
#define NIRVANA_ORB_CORE_PROXYMANAGER_H_

#include <CORBA/Server.h>
#include "../AtomicCounter.h"
#include "GarbageCollector.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyManager :
	public Bridge <Object>
{
protected:
	ProxyManager (const Bridge <Object>::EPV& proxy_impl, AbstractBase_ptr servant);

public:
	void _add_ref ()
	{
		if (1 == ref_cnt_.increment ()) {
			try {
				interface_duplicate (servant_);
			} catch (...) {
				ref_cnt_.decrement ();
				throw;
			}
		}
	}

	void _remove_ref ()
	{
		if (!ref_cnt_.decrement ()) {
			::Nirvana::DeadlineTime dt = ::Nirvana::g_current->next_async_deadline ();
			{
				::Nirvana::Runnable_var gc = (new GarbageCollector (servant_))->_get_ptr ();
				::Nirvana::g_current->next_async_deadline (::Nirvana::INFINITE_DEADLINE);
				try {
					sync_domain_->async_call (gc);
				} catch (...) {
					::Nirvana::g_current->next_async_deadline (dt);
					throw;
				}
			}
			::Nirvana::g_current->next_async_deadline (dt);
		}
	}

	// AbstractBase

	Interface_ptr _query_interface (const String& iid);

	// ServantBase default implementation

	InterfaceDef_ptr default_get_interface () const
	{
		return InterfaceDef::_nil ();	// TODO: Implement
	}

	Boolean default_is_a (const String& type_id) const
	{
		Interface* itf = servant_->_query_interface (type_id);
		if (itf)
			return true;
		else
			return false;
	}

	static Boolean default_non_existent ()
	{
		return false;
	}

	// Object equivalency default implementation

	Boolean default_is_equivalent (Object_ptr other) const
	{
		return (static_cast <Bridge <Object>*> (other) == this);
	}

	// Other methods

	const ProxyManager& defaults () const
	{
		return *this;
	}

	Interface_ptr primary_interface () const
	{
		return servant_->_query_interface (0);
	}

	const Char* primary_interface_id () const
	{
		return primary_interface ()->_epv ().interface_id;
	}

	::Nirvana::SyncDomainTraits_ptr sync_domain () const
	{
		return sync_domain_;
	}

private:
	AbstractBase_ptr servant_;
	::Nirvana::Core::AtomicCounter ref_cnt_;
	::Nirvana::SyncDomainTraits_var sync_domain_;
};

}
}
}

#endif
