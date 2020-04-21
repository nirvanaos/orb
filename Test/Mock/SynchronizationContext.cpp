#include <Core/SynchronizationContext.h>

namespace Nirvana {
namespace Core {

class FreeSyncContext :
	public SynchronizationContext
{
public:
	virtual void enter (bool ret)
	{}

	virtual void async_call (Runnable* runnable, DeadlineTime)
	{
		runnable->run ();
	}

	virtual bool synchronized ()
	{
		return false;
	}

	virtual Pointer allocate (Size& size)
	{
		Pointer p = g_memory->allocate (0, size, 0);
		Size au = (Size)g_memory->query (p, MemQuery::ALLOCATION_UNIT);
		size = round_up (size, au);
		return p;
	}

	virtual Pointer copy (ConstPointer src, Size& size)
	{
		Pointer p = g_memory->copy (0, (Pointer)src, size, 0);
		Size au = (Size)g_memory->query (p, MemQuery::ALLOCATION_UNIT);
		size = round_up (size, au);
		return p;
	}

	virtual void release (Pointer p, Size size)
	{
		g_memory->release (p, size);
	}

	virtual bool shared_memory (SynchronizationContext* other)
	{
		return true;
	}

	virtual void core_add_ref ()
	{}

	virtual void core_remove_ref ()
	{}
};

Core_var <SynchronizationContext> SynchronizationContext::current ()
{
	static FreeSyncContext singleton;
	return &singleton;
}

Core_var <SynchronizationContext> SynchronizationContext::not_synchronized ()
{
	return current ();
}

}
}
