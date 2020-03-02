#include <Nirvana/SyncDomainTraits_s.h>

namespace Nirvana {
namespace Core {

class FreeSyncDomain :
	public CORBA::Nirvana::ServantStatic <FreeSyncDomain, SyncDomainTraits>
{
public:
	static void* copy_inout (const void* src, size_t size)
	{
		return g_memory->copy (0, const_cast <void*> (src), size, 0);
	}

	static void* move_out (void* src, size_t size)
	{
		return src;
	}

	void enter (ContextFrame& context)
	{}

	void leave (const ContextFrame& context)
	{}

	void async_call (Runnable_ptr runnable)
	{
		runnable->run ();
	}

	static bool synchronized ()
	{
		return false;
	}
};

extern const CORBA::Nirvana::StaticI_ptr <SyncDomainTraits> g_free_sync_domain = { STATIC_BRIDGE (FreeSyncDomain, SyncDomainTraits) };

}
}

