#ifndef NIRVANA_ORB_STRINGMARSHAL_H_
#define NIRVANA_ORB_STRINGMARSHAL_H_

#include "StringABI.h"
#include "LocalMarshal_c.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
void StringABI <C>::marshal (StringABI& dst) const
{
	C* p;
	size_t len;
	if (is_large ()) {
		p = large_pointer ();
		len = large_size ();
	} else {
		p = small_pointer ();
		len = small_size ();
	}
	if (len > SMALL_CAPACITY) {
		size_t cb = (len + 1) * sizeof (C);
		dst.large_pointer ((C*)LocalMarshal::singleton ()->marshal_memory (large_pointer (), cb));
		dst.large_size (len);
		dst.large_allocated (cb);
	} else {
		::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
		dst.small_size (len);
	}
}

}
}

#endif
