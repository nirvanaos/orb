/// \file ABI_String.h.
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_ABI_STRING_H_
#define NIRVANA_ORB_ABI_STRING_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include "ABI.h"
#include <assert.h>
#include <stddef.h>

namespace std {
template <class T> class allocator;
template <class C, class T, class A> class basic_string;
template <class C> struct char_traits;
template <typename C, class T> class basic_string <C, T, allocator <C> >;
}

namespace CORBA {
namespace Internal {

template <typename C>
using StringT = std::basic_string <C, std::char_traits <C>, std::allocator <C> >;

/// The string ABI.
/// 
/// \tparam C Character type.
template <typename C>
struct alignas (sizeof (void*)) ABI <StringT <C> >
{
	static size_t max_size () NIRVANA_NOEXCEPT
	{
		return (SIZE_MAX / 2 + 1) / sizeof (C) - 1;
	}

	size_t size () const NIRVANA_NOEXCEPT
	{
		if (is_large ())
			return large_size ();
		else
			return small_size ();
	}

	size_t capacity () const NIRVANA_NOEXCEPT
	{
		if (is_large ())
			return large_capacity ();
		else
			return SMALL_CAPACITY;
	}

	bool empty () const NIRVANA_NOEXCEPT
	{
		return size () == 0;
	}

	const C* _ptr () const NIRVANA_NOEXCEPT
	{
		if (is_large ())
			return large_pointer ();
		else
			return small_pointer ();
	}

	C* _ptr () NIRVANA_NOEXCEPT
	{
		if (is_large ())
			return large_pointer ();
		else
			return small_pointer ();
	}

	void reset () NIRVANA_NOEXCEPT
	{
		size_t* p = data.raw, *end = p + countof (data.raw);
		do {
			*p = 0;
		} while (end != ++p);
	}

	bool is_large () const NIRVANA_NOEXCEPT
	{
		return (data.small.size & SMALL_MASK) != 0;
	}

	void small_size (size_t s) NIRVANA_NOEXCEPT
	{
		assert (s <= SMALL_CAPACITY);
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data.small.size = (unsigned char)(s << 1);
		else
			data.small.size = (unsigned char)(s);
		assert (!is_large ());
	}

	size_t small_size () const NIRVANA_NOEXCEPT
	{
		assert (!is_large ());
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			return data.small.size >> 1;
		else
			return data.small.size;
	}

	void large_size (size_t s) NIRVANA_NOEXCEPT
	{
		data.large.size = s;
	}

	size_t large_size () const NIRVANA_NOEXCEPT
	{
		assert (is_large ());
		return data.large.size;
	}

	void allocated (size_t cb) NIRVANA_NOEXCEPT
	{
		assert (!(cb & 2));
		if (::Nirvana::endian::native == ::Nirvana::endian::big)
			data.large.allocated = LARGE_MASK | cb;
		else
			data.large.allocated = LARGE_MASK | (cb >> 1);
		assert (is_large ());
	}

	size_t allocated () const NIRVANA_NOEXCEPT
	{
		size_t space = data.large.allocated;
		if (space & LARGE_MASK) {
			if (::Nirvana::endian::native == ::Nirvana::endian::big)
				return space & ~LARGE_MASK;
			else
				return space << 1;
		} else
			return 0;
	}

	size_t large_capacity () const NIRVANA_NOEXCEPT
	{
		assert (is_large ());
		size_t space = allocated ();
		if (space)
			return space / sizeof (C) - 1;
		else
			return large_size ();
	}

	void large_pointer (C* p) NIRVANA_NOEXCEPT
	{
		data.large.data = p;
	}

	const C* large_pointer () const NIRVANA_NOEXCEPT
	{
		return data.large.data;
	}

	C* large_pointer () NIRVANA_NOEXCEPT
	{
		return data.large.data;
	}

	const C* small_pointer () const NIRVANA_NOEXCEPT
	{
		return data.small.data;
	}

	C* small_pointer () NIRVANA_NOEXCEPT
	{
		return data.small.data;
	}

	static const unsigned char SMALL_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 0x01 : 0x80;
	static const size_t LARGE_MASK = ::Nirvana::endian::native == ::Nirvana::endian::big ? 1 : ~(size_t (~0) >> 1);

	struct Large
	{
		C* data;
		size_t size; ///< Element count
		size_t allocated; ///< Allocated space in bytes
	};

	static_assert (sizeof (Large) == sizeof (void*) + 2 * sizeof (size_t), "sizeof (ABI <StringT <C>>::Large)");

	struct Small
	{
		union
		{
			C data [(sizeof (Large) - 1) / sizeof (C)];
			struct
			{
				unsigned char padding [sizeof (Large) - 1];
				unsigned char size; ///< Element count
			};
		};
	};

	static_assert (offsetof (Small, size) == sizeof (Large) - 1, "ABI <StringT <C>>::Small");
	static_assert (sizeof (Large) == sizeof (Small), "ABI <StringT <C>>::Small");

	static const size_t SMALL_CAPACITY = sizeof (Small::data) / sizeof (C) - 1;

	union ULS
	{
		Small small;
		Large large;
	};

	union alignas (sizeof (void*)) Data
	{
		Small small;
		Large large;
		size_t raw [sizeof (ULS) / sizeof (size_t)];
	};

	Data data;
};

}
}

#endif
