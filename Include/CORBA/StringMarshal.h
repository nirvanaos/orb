#ifndef NIRVANA_ORB_STRINGMARSHAL_H_
#define NIRVANA_ORB_STRINGMARSHAL_H_

#include <Nirvana/basic_string.h>
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

template <typename C>
void StringABI <C>::unmarshal_inout () const
{
	if (is_large ()) {
		size_t cb = large_allocated ();
		if (cb)
			LocalMarshal::singleton ()->unmarshal_memory (large_pointer (), cb);
	}
}

}
}

namespace std {

template <typename C>
static basic_string <C>& basic_string <C>::unmarshal (::CORBA::Nirvana::StringABI <C>* abi)
{
	::CORBA::Nirvana::_check_pointer (abi);
	return static_cast <basic_string <C>&> (*abi);
}

template <typename C>
static const basic_string <C>& basic_string <C>::unmarshal (const ::CORBA::Nirvana::StringABI <C>* abi)
{
	return unmarshal (const_cast <::CORBA::Nirvana::StringABI <C>*> (abi));
}

}

#endif
