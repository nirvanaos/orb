#ifndef NIRVANA_ORB_CORE_GARBAGECOLLECTOR_H_
#define NIRVANA_ORB_CORE_GARBAGECOLLECTOR_H_

#include "../Runnable.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class GarbageCollector :
	public ::Nirvana::Core::Runnable <GarbageCollector>
{
public:
	GarbageCollector (Interface* itf) :
		itf_ (itf)
	{}

	void run ()
	{
		interface_release (itf_);
	}

private:
	Interface* itf_;
};

}
}
}

#endif
