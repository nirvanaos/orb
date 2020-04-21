#include <Nirvana/core_objects.h>
#include <Nirvana/Current_s.h>

namespace Nirvana {

namespace Core {

class Current :
	public CORBA::Nirvana::ServantStatic <Current, Nirvana::Current>
{
public:
	static DeadlineTime deadline ()
	{
		return INFINITE_DEADLINE;
	}
};

}

extern const ImportInterfaceT <Current> g_current = { 0, 0, 0, STATIC_BRIDGE (Core::Current, Nirvana::Current) };

}
