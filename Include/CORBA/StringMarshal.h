//! \file StringMarshal.h.
//!
//! \brief String marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_STRINGMARSHAL_H_
#define NIRVANA_ORB_STRINGMARSHAL_H_

#include <Nirvana/basic_string.h>
#include "LocalMarshal_c.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
void StringABI <C>::_marshal (StringABI& dst) const
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

namespace std {

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_adopt ()
{
	size_t cb = this->allocated ();
	if (cb)
		LocalMarshal::singleton ()->adopt_memory (this->large_pointer (), cb);
	else if (this->is_large ())
		assign (this->large_pointer (), this->large_size ());
}

}

#endif
