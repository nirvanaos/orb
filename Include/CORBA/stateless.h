#ifndef NIRVANA_ORB_STATELESS_H_
#define NIRVANA_ORB_STATELESS_H_

#include <Nirvana/core_objects.h>
#include "Servant_var.h"
#include <utility>

namespace CORBA {
namespace Nirvana {

template <class T, class ... Args>
inline PortableServer::Servant_var <T> stateless_create (Args ... args)
{
	Octet [sizeof (T)] tmp;
	StatelessCreationStruct scb { tmp, sizeof (T) };
	g_object_factory->stateless_begin (scb);
	try {
		new (tmp) T (std::forward <Args> (args));
		return (T*)g_object_factory->stateless_end (scb, true);
	} catch (...) {
		g_object_factory->stateless_end (scb, false);
		throw;
	}
}

inline const void* stateless_copy (const void* src, size_t size)
{
	return g_object_factory->stateless_copy (src, size);
}

inline void stateless_free (const void* src, size_t size)
{
	g_object_factory->memory_release (const_cast <void*> (src), size);
}

}
}

#endif
