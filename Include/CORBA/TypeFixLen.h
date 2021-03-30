/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeByVal.h"
#include "TypeByRef.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
template <class T
#ifdef NIRVANA_C11
	, typename Enable = void
#endif
>
using TypeFixLen = typename std::conditional <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T>, TypeByRef <T, T> >::type;

template <class T> struct Type :
	TypeFixLen <T
#ifdef NIRVANA_C11
	, typename std::enable_if <std::is_trivially_copyable <T>::value>::type
#endif
	>
{};

}
}

#endif
