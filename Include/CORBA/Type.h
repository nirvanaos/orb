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
#ifndef NIRVANA_ORB_TYPE_H_
#define NIRVANA_ORB_TYPE_H_

#include "TypeFixLen.h"
#include "TypeEnum.h"
#include "Type_interface.h"
#include "Client.h"
#include <Nirvana/ImportInterface.h>
#include "UserException.h"
#include "Any.h"
#include "Boolean.h"
#include "EnvironmentEx.h"
#include "TypeCode.h"
#include "Marshal.h"
#include "Unmarshal.h"
#include "tc_constants.h"
#include "Sequence.h"
#include "Object.h"

namespace CORBA {
namespace Internal {

/// The `void` type.
template <>
struct Type <void>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_void;
	}
};

// Interface marshaling

template <class I> inline
void TypeItf <I>::unmarshal (Interface* src, Unmarshal_ptr unmarshaler, I_ref <I>& dst)
{
	dst = unmarshaler->unmarshal_interface <I> (src);
}

template <class I>
void TypeItf <I>::marshal_in (I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	if (marshaler->marshal_context () < MarshalContext::OTHER_PROTECTION_DOMAIN)
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (src);
	else
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (Object::_ptr_type (src));
}

// TypeCode marshaling
template <> inline
void TypeItf <TypeCode>::marshal_in (I_ptr <TypeCode> src, Marshal_ptr marshaler, Interface*& dst)
{
	reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (src);
}

// String marshaling

template <typename C>
void Type <StringT <C> >::marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (const_cast <C*> (src.large_pointer ()), cb, 0));
		dst.large_size (size);
		dst.allocated (cb);
	}
}

template <typename C>
void Type <StringT <C> >::marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (src.large_pointer (), cb, src.allocated ()));
		dst.large_size (size);
		dst.allocated (cb);
		src.reset ();
	}
}

template <typename C>
void Type <StringT <C> >::unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Var& dst)
{
	if (Base::has_check)
		check (src);

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

// Sequence marshaling

template <typename T>
void Type <Sequence <T> >::marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		const ABI& asrc = static_cast <const ABI&> (src);
		if (!Type <T>::has_marshal) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, 0);
			dst.allocated = cb;
			dst.size = asrc.size;
		} else {
			size_t cb = asrc.size * sizeof (T);
			void* buf;
			dst.ptr = (T_ABI*)marshaler->get_buffer (cb, buf);
			T_ABI* dp = (T_ABI*)buf;
			const T* sp = src.data (), * end = sp + src.size ();
			do {
				Type <T>::marshal_in (*(sp++), marshaler, *(dp++));
			} while (sp != end);
		}
	}
}

template <typename T>
void Type <Sequence <T> >::marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		ABI& asrc = static_cast <ABI&> (src);
		dst.size = asrc.size;
		if (!Type <T>::has_marshal) {
			size_t cb = asrc.size * sizeof (T);
			dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated);
			dst.allocated = cb;
			asrc.reset ();
		} else {
			T* sp = src.data (), * end = sp + src.size ();
			T_ABI* dp;
			size_t cb;
			MarshalContext mctx = marshaler->marshal_context ();
			if (MarshalContext::SHARED_MEMORY == mctx)
				dp = (T_ABI*)sp;
			else {
				cb = asrc.size * sizeof (T);
				void* buf;
				dst.ptr = (T_ABI*)marshaler->get_buffer (cb, buf);
				dp = (T_ABI*)buf;
			}

			do {
				Type <T>::marshal_out (*(sp++), marshaler, *(dp++));
			} while (sp != end);

			if (MarshalContext::SHARED_MEMORY == mctx)
				dst.ptr = (T_ABI*)marshaler->marshal_memory (asrc.ptr, cb, asrc.allocated);
			else {
				Var tmp;
				src.swap (tmp);
			}

			dst.allocated = cb;
		}
	}
}

template <typename T>
void Type <Sequence <T> >::unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Var& dst)
{
	if (Base::has_check)
		check (src);

	if (!src.size)
		dst.reset ();
	else {
		Var tmp;
		ABI& adst = static_cast <ABI&> (tmp);
		if (src.allocated) {
			unmarshaler->adopt_memory (src.ptr, src.allocated);
			adst.ptr = src.ptr;
			adst.allocated = src.allocated;
		} else
			adst.ptr = (T*)::Nirvana::MemoryHelper::assign (0, adst.allocated, 0, src.size * sizeof (T), src.ptr);
		if (Type <T>::has_marshal) {
			T_ABI* sp = src.ptr, * end = sp + src.size;
			T* dp = adst.ptr;
			try {
				do {
					Type <T>::unmarshal (*(sp++), unmarshaler, *(dp++));
				} while (sp != end);
			} catch (...) {
				adst.size = dp - adst.ptr;
				throw;
			}
		}
		adst.size = src.size;
		tmp.swap (dst);
	}
}

}
}

// IDL::traits
namespace IDL {

template <class T> struct traits;

}

#endif
