#include <Nirvana/core_objects.h>
#include <Nirvana/Current_s.h>

namespace Nirvana {

namespace Core {

extern const CORBA::Nirvana::StaticI_ptr <SyncDomainTraits> g_free_sync_domain;

class Current :
	public CORBA::Nirvana::ServantStatic <Current, Nirvana::Current>
{
public:
	static SyncDomainTraits_ptr sync_domain_traits ()
	{
		return g_free_sync_domain;
	}

	static DeadlineTime deadline ()
	{
		return INFINITE_DEADLINE;
	}

	static DeadlineTime set_next_async_deadline (DeadlineTime)
	{
		return INFINITE_DEADLINE;
	}
};

}

extern const ImportInterfaceT <Current> g_current = { 0, 0, 0, STATIC_BRIDGE (Core::Current, Nirvana::Current) };

}
