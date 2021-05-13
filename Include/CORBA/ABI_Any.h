/// \file ABI_Any.h.
///
/// \brief Declares the any ABI.
/// This file have not be changed in future

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
#ifndef NIRVANA_ORB_ABI_ANY_H_
#define NIRVANA_ORB_ABI_ANY_H_

#include <Nirvana/NirvanaBase.h>
#include "ABI.h"
#include <assert.h>

namespace CORBA {

class Any;
class TypeCode;

namespace Internal {

template <>
struct alignas (sizeof (uintptr_t)) ABI <Any>
{
	/// Pointer to TypeCode and large data flag in the least significant bit.
	uintptr_t type_code;

	struct alignas (sizeof (void*))
	{
		void* p;	//< Pointer to large data
		size_t size;	//< Large data size
		size_t padding;
	} data;

	static const size_t SMALL_CAPACITY = sizeof (data);

	void reset ()
	{
		type_code = 0;
	}

	TypeCode* type () const
	{
		return reinterpret_cast <TypeCode*> (type_code & ~1);
	}

	void type (void* tc)
	{
		assert (((uintptr_t)tc & 1) == 0);
		type_code = reinterpret_cast <uintptr_t> (tc) | (type_code & 1);
	}

	bool is_large () const
	{
		return (type_code & 1) != 0;
	}

	const void* small_pointer () const
	{
		return &data;
	}

	void* small_pointer ()
	{
		return &data;
	}

	void* large_pointer () const
	{
		return data.p;
	}

	void large_pointer (void* p, size_t size)
	{
		data.p = p;
		data.size = size;
		type_code |= 1;
	}

	size_t large_size () const
	{
		return data.size;
	}
};

}
}

#endif
