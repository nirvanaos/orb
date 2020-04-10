#include <Core/SynchronizationContext.h>

namespace Nirvana {
namespace Core {

class FreeSyncContext :
	public CORBA::Nirvana::ServantStatic <FreeSyncContext, SynchronizationContext>
{
public:
	static void enter_memory ()
	{}

	static void enter (bool ret)
	{}

	static Pointer adopt_output (Pointer src, Size data_size, Size& allocated_size)
	{
		return src;
	}

	static Pointer allocate (Size& size)
	{
		Pointer p = g_memory->allocate (0, size, 0);
		Size au = (Size)g_memory->query (p, MemQuery::ALLOCATION_UNIT);
		size = round_up (size, au);
		return p;
	}

	void async_call (Runnable_ptr runnable)
	{
		runnable->run ();
	}

	static bool synchronized ()
	{
		return false;
	}

	static bool shared_memory (SynchronizationContext_ptr)
	{
		return true;
	}
};

extern const CORBA::Nirvana::StaticI_ptr <SynchronizationContext> g_free_sync_context = { STATIC_BRIDGE (FreeSyncContext, SynchronizationContext) };

}
}

