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
#include "Proxy/Marshal.h"
#include "Proxy/Unmarshal.h"
#include "tc_constants.h"
#include "Sequence.h"
#include "Object.h"

namespace CORBA {
namespace Nirvana {

/// The `void` type.
template <>
struct Type <void>
{
	static TypeCode_ptr type_code ()
	{
		return _tc_void;
	}
};

inline
TypeCode_ptr Type <Any>::type_code ()
{
	return _tc_any;
}

inline
TypeCode_ptr Type <I_var <TypeCode> >::type_code ()
{
	return _tc_TypeCode;
}

// Interface marshaling

template <class I> inline
void TypeItf <I>::marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (src);
}

template <class I> inline
void TypeItf <I>::marshal_out (I_var <I>& src, Marshal_ptr marshaler, Interface*& dst)
{
	marshal_in (src, marshaler, dst);
}

template <class I> inline
void TypeItf <I>::unmarshal (Interface* src, Unmarshal_ptr unmarshaler, I_var <I>& dst)
{
	dst = static_cast <I*> (unmarshaler->unmarshal_interface (src, I::repository_id_));
}

// Object marshaling

template <class I>
void TypeObject <I>::marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	if (marshaler->context () < MarshalContext::OTHER_PROTECTION_DOMAIN)
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (src);
	else
		reinterpret_cast <uintptr_t&> (dst) = marshaler->marshal_interface (Object_ptr (src));
}

// String marshaling

template <typename C>
void Type <StringT <C> >::marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var_type::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (const_cast <C*> (src.large_pointer ()), cb, 0));
		dst.large_size (size);
		dst.allocated (cb);
	}
}

template <typename C>
void Type <StringT <C> >::marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var_type::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (src.large_pointer (), cb, src.allocated ()));
		dst.large_size (size);
		dst.allocated (cb);
		src.reset ();
	}
}

template <typename C>
void Type <StringT <C> >::unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst)
{
	if (Base::has_check)
		check (src);

	if (src.is_large ()) {
		size_t cb = src.allocated ();
		if (cb) {
			unmarshaler->adopt_memory (src.large_pointer (), cb);
			static_cast <ABI_type&> (dst) = src;
		} else
			dst.assign_internal (src.large_size (), src.large_pointer ());
	} else
		static_cast <ABI_type&> (dst) = src;
}

// Sequence marshaling

template <typename T>
void Type <Sequence <T> >::marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		const ABI_type& asrc = static_cast <const ABI_type&> (src);
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
void Type <Sequence <T> >::marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (src.empty ())
		dst.reset ();
	else {
		ABI_type& asrc = static_cast <ABI_type&> (src);
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
			MarshalContext mctx = marshaler->context ();
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
				Var_type tmp;
				src.swap (tmp);
			}

			dst.allocated = cb;
		}
	}
}

template <typename T>
void Type <Sequence <T> >::unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst)
{
	if (Base::has_check)
		check (src);

	if (!src.size)
		dst.reset ();
	else {
		Var_type tmp;
		ABI_type& adst = static_cast <ABI_type&> (tmp);
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

// Marshaling helpers

template <class T, class ABI> inline
void _marshal_in (const T& src, Marshal_ptr marshaler, ABI& dst)
{
	Type <T>::marshal_in (src, marshaler, dst);
}

template <class I> inline
void _marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	Type <I_var <I> >::marshal_in (src, marshaler, dst);
}

inline
void _marshal_in (const TypeCode_ptr src, Marshal_ptr marshaler, Interface*& dst)
{
	Type <TypeCode_var>::marshal_in (src, marshaler, dst);
}

template <class T, class ABI> inline
void _marshal_out (T& src, Marshal_ptr marshaler, ABI& dst)
{
	Type <T>::marshal_out (src, marshaler, dst);
}

template <class T, class ABI> inline
void _unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, T& dst)
{
	Type <T>::unmarshal (src, unmarshaler, dst);
}

}
}

// Compatibility with modern C11 mapping
namespace IDL {

template <class T>
struct traits
{
	typedef typename CORBA::Nirvana::Type <T>::C_var value_type;
	typedef typename CORBA::Nirvana::Type <T>::C_in in_type;
	typedef typename CORBA::Nirvana::Type <T>::C_out out_type;
	typedef typename CORBA::Nirvana::Type <T>::C_inout inout_type;
};

}

#endif
