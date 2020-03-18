#include "ProxyManager.h"
#include "../Runnable.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyManager::GarbageCollector :
	public ::Nirvana::Core::Runnable <GarbageCollector>
{
public:
	GarbageCollector (Interface_ptr servant) :
		servant_ (servant)
	{}

	~GarbageCollector ()
	{}

	void run ()
	{
		release (servant_);
	}

private:
	Interface_ptr servant_;
};

ProxyManager::ProxyManager (const Bridge <Object>::EPV& proxy_impl, AbstractBase_ptr servant, Interface_ptr lifecycle) :
	Bridge <Object> (proxy_impl),
	servant_ (servant),
	servant_lifecycle_ (lifecycle),
	ref_cnt_ (0),
	sync_context_ (::Nirvana::SynchronizationContext::_duplicate (::Nirvana::g_current->synchronization_context ()))
{}

Interface_ptr ProxyManager::_query_interface (const String& iid) const
{
	if (!iid.empty ()) {
		switch (RepositoryId::check (Object::repository_id_, iid)) {
			case RepositoryId::COMPATIBLE:
				return &const_cast <ProxyManager&> (*this);
			case RepositoryId::INCOMPATIBLE_VERSION:
				return 0;
		}
	}

	// Real implementation must return proxy
	// For test we just shortcut to servant
	return servant_->_query_interface (iid);
}

void ProxyManager::add_ref_1 ()
{
	interface_duplicate (&servant_lifecycle_);
}

::Nirvana::Core::AtomicCounter::UIntType ProxyManager::_remove_ref ()
{
	::Nirvana::Core::AtomicCounter::IntType cnt = ref_cnt_.decrement ();
	if (!cnt) {
		try {
			run_garbage_collector ((new GarbageCollector (servant_lifecycle_))->_get_ptr ());
		} catch (...) {
			// Async call failed, maybe resources are exausted.
			// Fallback to collect garbage in current thread.
			::Nirvana::Synchronized sync (sync_context_);
			release (servant_lifecycle_);
			// Swallow exception
		}
	} else if (cnt < 0) {
		// TODO: Log error
		ref_cnt_.increment ();
		cnt = 0;
	}
		
	return cnt;
}

void ProxyManager::run_garbage_collector (::Nirvana::Runnable_var gc) const
{
	::Nirvana::DeadlineTime cur_dt = ::Nirvana::g_current->set_next_async_deadline (::Nirvana::INFINITE_DEADLINE);
	try {
		sync_context_->async_call (gc);
	} catch (...) {
		::Nirvana::g_current->set_next_async_deadline (cur_dt);
		throw;
	}
	::Nirvana::g_current->set_next_async_deadline (cur_dt);
}

}
}
}
