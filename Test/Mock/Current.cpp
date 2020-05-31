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

extern const ImportInterfaceT <Current> g_current = { OLF_IMPORT_INTERFACE, 0, 0, STATIC_BRIDGE (Nirvana::Current, Core::Current) };

}
