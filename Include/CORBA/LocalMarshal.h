//! \file StringMarshal.h.
//!
//! \brief String marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_LOCALMARSHAL_H_
#define NIRVANA_ORB_LOCALMARSHAL_H_

#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include "LocalMarshal_c.h"
#include <Nirvana/Memory_c.h>

namespace std {

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_local_marshal (basic_string& dst) const
{
	const C* p;
	size_t len;
	if (this->is_large ()) {
		p = this->large_pointer ();
		len = this->large_size ();
	} else {
		p = this->small_pointer ();
		len = this->small_size ();
	}
	if (len > ABI::SMALL_CAPACITY) {
		size_t cb = byte_size (len);
		dst.large_pointer ((C*)CORBA::Nirvana::g_local_marshal->marshal_memory (p, cb));
		dst.large_size (len);
		dst.allocated (cb);
	} else {
		::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
		dst.small_size (len);
	}
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::_local_unmarshal ()
{
	if (this->is_large ()) {
		size_t cb = this->allocated ();
		if (cb)
			CORBA::Nirvana::g_local_marshal->adopt_memory (this->large_pointer (), cb);
		else
			assign_internal (this->large_size (), this->large_pointer ());
	}
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

	static void local_marshal (const T& src, T& dst);
	static void local_unmarshal (T& val);
};

template <typename C, class T>
struct MarshalTraits <std::basic_string <C, T, std::allocator <C> > >
{
	static bool is_dynamic ()
	{
		return true;
	}

	static void local_marshal (const std::basic_string <C, T, std::allocator <C> >& src, std::basic_string <C, T, std::allocator <C> >& dst)
	{
		src._local_marshal (dst);

	}
	static void local_unmarshal (std::basic_string <C, T, std::allocator <C> >& val)
	{
		val._local_unmarshal ();
	}
};

template <typename T>
struct MarshalTraits <std::vector < T, std::allocator <T> > >
{
	static bool is_dynamic ()
	{
		return true;
	}

	static void local_marshal (const std::vector < T, std::allocator <T> >& src, std::vector < T, std::allocator <T> >& dst)
	{
		if (src.empty ())
			dst.reset ();
		else if (std::is_trivially_copyable <T> ()) {
			dst.data_.allocated = (dst.data_.size = src.size ()) * sizeof (T);
			dst.data_.ptr = (T*)g_local_marshal->marshal_memory (src.data (), dst.data_.allocated);
		} else {
			size_t cb = src.size () * sizeof (T);
			void* buf;
			dst.data_.ptr = (T*)g_local_marshal->get_buffer (cb, buf);
			T* dp = (T*)buf;
			const T* sp = src.data (), *end = sp + src.size ();
			do {
				MarshalTraits <T>::local_marshal (*(sp++), *(dp++));
			} while (sp != end);
		}
	}

	static void local_unmarshal (std::vector < T, std::allocator <T> >& val)
	{
		if (!val.empty ()) {
			if (val.data_.allocated)
				g_local_marshal->adopt_memory (val.data_.ptr, val.data_.allocated);
			else
				val.data_.ptr = (T*)heap ()->copy (nullptr, val.data_.ptr, val.data_.allocated, 0);
			if (!std::is_trivially_copyable <T> ()) {
				T* p = val.data (), *end = p + val.size (); 
				do {
					MarshalTraits <T>::local_unmarshal (*(p++));
				} while (p != end);
			}
		}
	}
};

}
}

#endif
