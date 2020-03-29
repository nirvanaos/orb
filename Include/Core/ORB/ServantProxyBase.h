/// \file ServantProxyBase.h
#ifndef NIRVANA_ORB_CORE_SERVANTPROXYBASE_H_
#define NIRVANA_ORB_CORE_SERVANTPROXYBASE_H_

#include "ProxyManager.h"
#include <CORBA/AbstractBase_s.h>
#include <CORBA/Object_s.h>
#include <CORBA/Proxy/IOReference_s.h>
#include <Core/AtomicCounter.h>
#include <Core/Synchronized.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// \brief Base for servant-side proxies.
class ServantProxyBase :
	public ServantTraits <ServantProxyBase>,
	public LifeCycleRefCnt <ServantProxyBase>,
	public InterfaceImplBase < ServantProxyBase, AbstractBase>,
	public Skeleton <ServantProxyBase, Object>,
	public Skeleton <ServantProxyBase, IOReference>,
	public ProxyManager
{
	class GarbageCollector;
public:
	Bridge <Object>* _get_object (String_in iid)
	{
		if (RepositoryId::check (Object::repository_id_, iid) != RepositoryId::COMPATIBLE)
			::Nirvana::throw_INV_OBJREF ();
		return this;
	}

	void _add_ref ()
	{
		::Nirvana::Core::AtomicCounter::IntType cnt = ref_cnt_.increment ();
		if (1 == cnt) {
			try {
				::Nirvana::Core::Synchronized sync (sync_context_);
				add_ref_1 ();
			} catch (...) {
				ref_cnt_.decrement ();
				throw;
			}
		}
	}

	virtual ::Nirvana::Core::AtomicCounter::UIntType _remove_ref ();

protected:
	ServantProxyBase (AbstractBase_ptr servant, const Operation object_ops [3], void* object_impl);

	virtual void add_ref_1 ();

	::Nirvana::Core::SynchronizationContext* sync_context () const
	{
		return sync_context_;
	}

	void run_garbage_collector (::Nirvana::Core::Core_var <::Nirvana::Core::Runnable> gc) const
	{
		sync_context_->async_call (gc, ::Nirvana::INFINITE_DEADLINE);
	}

private:
	static const Char* primary_interface_id (AbstractBase_ptr servant)
	{
		Interface* primary = servant->_query_interface (0);
		if (!primary)
			throw OBJ_ADAPTER (); // TODO: Log
		return primary->_epv ().interface_id;
	}

private:
	AbstractBase_ptr servant_;
	::Nirvana::Core::AtomicCounter ref_cnt_;
	::Nirvana::Core::Core_var <::Nirvana::Core::SynchronizationContext> sync_context_;
};

}
}
}

#endif
