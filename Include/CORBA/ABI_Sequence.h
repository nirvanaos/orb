//! \file ABI_Sequence.h.
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
#ifndef NIRVANA_ORB_ABI_SEQUENCE_H_
#define NIRVANA_ORB_ABI_SEQUENCE_H_
#pragma once

namespace std {
template <class T, class A> class vector;
template <class T> class StdAllocator;
}

namespace CORBA {
namespace Internal {

template <typename T>
using Sequence = std::vector <T, std::StdAllocator <T> >;

/// Sequence ABI.
/// 
/// \tparam The sequence element type.
template <class T>
struct alignas (sizeof (void*)) ABI <Sequence <T> >
{
	void reset ()
	{
		ptr = nullptr;
		size = 0;
		allocated = 0;
	}

	T* ptr;
	size_t size; ///< Element count
	size_t allocated; ///< Allocated space in bytes
};

}
}

#endif
