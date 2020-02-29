#ifndef NIRVANA_ORB_CREATE_STATELESS_H_
#define NIRVANA_ORB_CREATE_STATELESS_H_

#include <Nirvana/core_objects.h>
#include <utility>

namespace CORBA {
namespace Nirvana {

template <class T, class ... Args>
inline T* create_stateless (Args ... args)
{
	Octet [sizeof (T)] tmp;
	StatelessCreationBlock scb { tmp, sizeof (T) };
	g_object_factory->stateless_begin (scb);
	try {
		new (tmp) T (std::forward <Args> (args));
		return (T*)g_object_factory->stateless_end (scb, true);
	} catch (...) {
		g_object_factory->stateless_end (scb, false);
		throw;
	}
}

}
}

#endif
