/// \file MarshalTraits_string.h
/// \brief Declares the MarshalTraits for strings.
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
#ifndef NIRVANA_ORB_MARSHALTRAITS_STRING_H_
#define NIRVANA_ORB_MARSHALTRAITS_STRING_H_

#include "MarshalTraits_forward.h"
#include "String.h"
#include "Proxy/Marshal.h"
#include "Proxy/Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
struct MarshalTraits <StringT <C> >
{
	static const bool has_marshal = true;

	typedef StringT <C> Var;
	typedef ABI <Var> ABI;

	static bool _small_copy (const ABI& src, ABI& dst);

	static void marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst);

	static void marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst);

	static void unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Var& dst)
	{
		if (Type <Var>::has_check)
			Type <Var>::check (src);

		if (src.is_large ()) {
			size_t cb = src.allocated ();
			if (cb) {
				unmarshaler->adopt_memory (src.large_pointer (), cb);
				static_cast <ABI&> (dst) = src;
			} else
				dst.assign_internal (src.large_size (), src.large_pointer ());
		} else
			static_cast <ABI&> (dst) = src;
	}
};

template <typename C>
void MarshalTraits <StringT <C> >::marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = StringT <C>::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (const_cast <C*> (src.large_pointer ()), cb, 0));
		dst.large_size (size);
		dst.allocated (cb);
	}
}

template <typename C>
void MarshalTraits <StringT <C> >::marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = StringT <C>::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (src.large_pointer (), cb, src.allocated ()));
		dst.large_size (size);
		dst.allocated (cb);
		src.reset ();
	}
}

template <typename C>
bool MarshalTraits <StringT <C> >::_small_copy (const ABI& src, ABI& dst)
{
	const C* p;
	size_t len;
	if (src.is_large ()) {
		len = src.large_size ();
		if (len > ABI::SMALL_CAPACITY)
			return false;
		p = src.large_pointer ();
	} else {
		p = src.small_pointer ();
		len = src.small_size ();
	}
	::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
	dst.small_size (len);
	return true;
}

}
}

#endif
