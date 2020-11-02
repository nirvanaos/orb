#ifndef NIRVANA_ORB_LIFECYCLESTATIC_H_
#define NIRVANA_ORB_LIFECYCLESTATIC_H_

#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

class LifeCycleStatic
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, Interface*)
	{
		::Nirvana::Module::_duplicate (::Nirvana::g_module);
		return itf;
	}

	template <class I>
	static void __release (Interface*)
	{
		release ((::Nirvana::Module_ptr)::Nirvana::g_module);
	}
};

}
}

#endif
