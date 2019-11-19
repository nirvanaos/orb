//! \file StringMarshal.h.
//!
//! \brief String marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_LOCALMARSHAL_H_
#define NIRVANA_ORB_LOCALMARSHAL_H_

#include <Nirvana/basic_string.h>
#include "LocalMarshal_c.h"

namespace std {

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_local_marshal (basic_string& dst) const
{
	C* p;
	size_t len;
	if (this->is_large ()) {
		p = this->large_pointer ();
		len = this->large_size ();
	} else {
		p = this->small_pointer ();
		len = this->small_size ();
	}
	if (len > ABI::SMALL_CAPACITY) {
		size_t cb = (len + 1) * sizeof (C);
		dst.large_pointer ((C*)LocalMarshal::singleton ()->marshal_memory (p, cb));
		dst.large_size (len);
		dst.large_allocated (cb);
	} else {
		::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
		dst.small_size (len);
	}
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_adopt ()
{
	size_t cb = this->allocated ();
	if (cb)
		LocalMarshal::singleton ()->adopt_memory (this->large_pointer (), cb);
	else if (this->is_constant_allocated ())
		assign_internal (this->large_size (), this->large_pointer ());
}

}

namespace CORBA {
namespace Nirvana {

template <class T>
struct MarshalTraits
{
	static bool is_dynamic ()
	{
		return false;
	}

	static void _local_marshal (const T& src, T& dst);
	static void _adopt (T& val);
};

template <typename C, class T>
struct MarshalTraits <std::basic_string <C, T, std::allocator <C> > >
{
	static bool is_dynamic ()
	{
		return true;
	}

	static void _local_marshal (const std::basic_string <C, T, std::allocator <C> >& src, std::basic_string <C, T, std::allocator <C> >& dst)
	{
		src._local_marshal (dst);

	}
	static void _adopt (std::basic_string <C, T, std::allocator <C> >& val)
	{
		val._adopt ();
	}
};

}
}

#endif
