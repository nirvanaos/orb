//! \file LocalMarshal.h.
//!
//! \brief Local marshaling functions for use in proxy code.

#ifndef NIRVANA_ORB_MARSHALTRAITS_H_
#define NIRVANA_ORB_MARSHALTRAITS_H_

#include <Nirvana/basic_string.h>
#include <Nirvana/vector.h>
#include "LocalMarshal.h"

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
void basic_string <C, T, allocator <C> >::_local_unmarshal_inout ()
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

template <class T> struct MarshalTraits;

template <class T>
struct MarshalTraits
{
	static const bool has_unmarshal_in_ = false;
	static const bool has_unmarshal_inout_ = false;

#ifdef NIRVANA_C11
	template <class = typename std::enable_if <std::is_trivially_copyable <T>::value>::type>
#endif
	static void local_marshal (const T& src, T& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}
	
	static void local_unmarshal_in (T& val) NIRVANA_NOEXCEPT
	{}

	static void local_unmarshal_inout (T& val) NIRVANA_NOEXCEPT
	{}
};

template <typename C, class T>
struct MarshalTraits <std::basic_string <C, T, std::allocator <C> > >
{
	static const bool has_unmarshal_in_ = false;
	static const bool has_unmarshal_inout_ = true;

	static void local_marshal (const std::basic_string <C, T, std::allocator <C> >& src, std::basic_string <C, T, std::allocator <C> >& dst)
	{
		src._local_marshal (dst);
	}

	static void local_unmarshal_in (std::basic_string <C, T, std::allocator <C> >& val) NIRVANA_NOEXCEPT
	{}

	static void local_unmarshal_inout (std::basic_string <C, T, std::allocator <C> >& val) NIRVANA_NOEXCEPT
	{
		val._local_unmarshal_inout ();
	}
};

template <typename T>
struct MarshalTraits <std::vector <T, std::allocator <T> > >
{
	static const bool has_unmarshal_in_ = T::has_unmarshal_in_;
	static const bool has_unmarshal_inout_ = true;

	static void local_marshal (const std::vector <T, std::allocator <T> >& src, std::vector <T, std::allocator <T> >& dst)
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

	static void local_unmarshal_in (std::vector <T, std::allocator <T> >& val)
	{
		if (MarshalTraits <T>::has_unmarshal_in_)
			for (T* p = val.data (), *end = p + val.size (); p != end; ++p)
				MarshalTraits <T>::local_unmarshal_in (*p);
	}

	static void local_unmarshal_inout (std::vector <T, std::allocator <T> >& val)
	{
		if (!val.empty ()) {
			if (val.data_.allocated)
				g_local_marshal->adopt_memory (val.data_.ptr, val.data_.allocated);
			else
				val.data_.ptr = (T*)val.memory ()->copy (nullptr, val.data_.ptr, val.data_.allocated, 0);
			if (MarshalTraits <T>::has_unmarshal_inout_) {
				T* p = val.data (), * end = p + val.size ();
				try {
					do {
						MarshalTraits <T>::local_unmarshal_inout (*p);
					} while (end != ++p);
				} catch (...) {
					val.data_.size = p - val.data_.ptr;
					val.~vector ();
					throw;
				}
			}
		}
	}
};

template <>
struct MarshalTraits <TypeCode_var>
{
	static const bool has_unmarshal_in_ = true;
	static const bool has_unmarshal_inout_ = true;

	static void local_marshal (TypeCode_ptr src, TypeCode_var& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = g_local_marshal->marshal_type_code (src);
	}

	static void local_unmarshal_in (TypeCode_var& val)
	{
		val = g_local_marshal->unmarshal_type_code (reinterpret_cast <void*&> (val));
	}

	static void local_unmarshal_inout (TypeCode_var& val)
	{
		local_unmarshal_in (val);
	}
};

template <class I> // I must derive from Object
struct MarshalTraits <I_var <I> >
{
	static const bool has_unmarshal_in_ = true;
	static const bool has_unmarshal_inout_ = true;

	static void local_marshal (Object_ptr src, I_var <I>& dst)
	{
		reinterpret_cast <uintptr_t&> (dst) = g_local_marshal->marshal_object (src);
	}

	static void local_unmarshal_in (I_var <I>& val)
	{
		Interface_ptr ip = g_local_marshal->unmarshal_interface (reinterpret_cast <void*&> (val), Bridge <I>::interface_id_);
		val = I_var <I> (static_cast <I*> (static_cast <Interface*> (ip)));
	}

	static void local_unmarshal_inout (I_var <I>& val)
	{
		local_unmarshal_in (val);
	}
};

}
}

#endif
