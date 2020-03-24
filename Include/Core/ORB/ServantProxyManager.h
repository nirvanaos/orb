#ifndef NIRVANA_ORB_CORE_SERVANTPROXYMANAGER_H_
#define NIRVANA_ORB_CORE_SERVANTPROXYMANAGER_H_

#include <CORBA/Server.h>
#include <Core/AtomicCounter.h>
#include <Core/Synchronized.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

class ServantProxyManager :
	public Bridge <Object>
{
	class GarbageCollector;
protected:
	ServantProxyManager (const Bridge <Object>::EPV& proxy_impl, AbstractBase_ptr servant, Interface_ptr lifecycle);

public:
	Bridge <Object>* _get_object (String_in iid)
	{
		if (RepositoryId::check (Object::repository_id_, iid) != RepositoryId::COMPATIBLE)
			::Nirvana::throw_INV_OBJREF ();
		return this;
	}

	Object_ptr _get_ptr ()
	{
		return &static_cast <Object&> (static_cast <Bridge <Object>&> (*this));
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

	virtual void add_ref_1 ();
	virtual ::Nirvana::Core::AtomicCounter::UIntType _remove_ref ();

	// AbstractBase

	Interface_ptr _query_interface (const String& iid) const;

	// ServantBase default implementation

	InterfaceDef_ptr default_get_interface () const
	{
		return InterfaceDef::_nil ();	// TODO: Implement
	}

	Boolean default_is_a (const String& type_id) const
	{
		Interface_ptr itf = _query_interface (type_id);
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
		return &other == this;
	}

	// Other methods

	const ServantProxyManager& defaults () const
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

	::Nirvana::Core::SynchronizationContext* sync_context () const
	{
		return sync_context_;
	}

	void run_garbage_collector (::Nirvana::Core::Core_var <::Nirvana::Core::Runnable> gc) const
	{
		sync_context_->async_call (gc, ::Nirvana::INFINITE_DEADLINE);
	}

private:
	AbstractBase_ptr servant_;
	Interface_ptr servant_lifecycle_;
	::Nirvana::Core::AtomicCounter ref_cnt_;
	::Nirvana::Core::Core_var <::Nirvana::Core::SynchronizationContext> sync_context_;
};

}
}
}

#endif