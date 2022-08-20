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
#ifndef NIRVANA_ORB_UNION_H_
#define NIRVANA_ORB_UNION_H_
#pragma once

#include "primitive_types.h"
#include <utility>

namespace CORBA {
namespace Internal {

template <class T> inline
void destruct (T& v) NIRVANA_NOEXCEPT
{
	v.~T ();
}

template <class T> inline
void construct (T& v) NIRVANA_NOEXCEPT
{
	::new (&v) T ();
}

template <class T> inline
void construct (T& v, const T& src)
{
	::new (&v) T (src);
}

inline
void construct (Octet& v, Boolean src)
{
	v = src;
}

template <class T> inline
void construct (T& v, T&& src) NIRVANA_NOEXCEPT
{
	::new (&v) T (std::move (src));
}

#ifndef LEGACY_CORBA_CPP

template <class I>
void construct (I_ref <I> v, I_ptr <I> src)
{
	::new (&v) I_ref <I> (src);
}

#else

template <class I>
void construct (I_var <I> v, I_ptr <I> src)
{
	::new (&v) I_var <I> (src);
}

#endif

template <> inline
void construct (Boolean& v) NIRVANA_NOEXCEPT
{
	v = false;
}

template <> inline
void construct (Char& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (WChar& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (Octet& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (Short& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (UShort& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (Long& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (ULong& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (LongLong& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

template <> inline
void construct (ULongLong& v) NIRVANA_NOEXCEPT
{
	v = 0;
}

}
}

#endif
