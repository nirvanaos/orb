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
#include <CORBA/MarshalTraits_any.h>

namespace CORBA {
namespace Nirvana {

void MarshalTraits <Any>::marshal_in (const Any& src, Marshal_ptr marshaler, ABI& dst)
{
	TypeCode_ptr tc = src.type ();
	if (!tc)
		dst.reset ();
	else {
		dst.type ((::Nirvana::Pointer)marshaler->marshal_interface (tc));
		::Nirvana::ConstPointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			size_t size = tc->_size ();
			uintptr_t p = marshaler->get_buffer (size, pdst);
			dst.large_pointer ((::Nirvana::Pointer)p, size);
			pdst = dst.large_pointer ();
			psrc = src.large_pointer ();
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
		}
		tc->_marshal_in (psrc, marshaler, pdst);
	}
}

void MarshalTraits <Any>::marshal_out (Any& src, Marshal_ptr marshaler, ABI& dst)
{
	TypeCode_ptr tc = src.type ();
	if (!tc)
		dst.reset ();
	else {
		dst.type ((::Nirvana::Pointer)marshaler->marshal_interface (tc));
		::Nirvana::Pointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			psrc = src.large_pointer ();
			if (MarshalContext::SHARED_MEMORY == marshaler->context ()) {
				dst = src;
				src.reset ();
				pdst = dst.large_pointer ();
			} else {
				size_t size = tc->_size ();
				uintptr_t p = marshaler->get_buffer (size, pdst);
				dst.large_pointer ((::Nirvana::Pointer)p, size);
				pdst = dst.large_pointer ();
			}
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
		}
		tc->_marshal_out (psrc, marshaler, pdst);
		release (tc);
		src.reset ();
	}
}

void MarshalTraits <Any>::unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Any& dst)
{
	TypeCode* ptc = src.type ();
	if (!ptc)
		dst.reset ();
	else {
		TypeCode_var tc (static_cast <TypeCode*> (unmarshaler->unmarshal_interface (ptc, TypeCode::repository_id_)));
		::Nirvana::ConstPointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			psrc = src.large_pointer ();
			size_t size = src.large_size ();
			if (size)
				unmarshaler->adopt_memory (pdst = const_cast <::Nirvana::Pointer> (psrc), size);
			else
				pdst = ::Nirvana::g_memory->allocate (0, size, 0);
			try {
				tc->_unmarshal (psrc, unmarshaler, pdst);
			} catch (...) {
				::Nirvana::g_memory->release (pdst, size);
				throw;
			}
			dst.large_pointer (pdst, size);
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
			tc->_unmarshal (psrc, unmarshaler, pdst);
		}
		dst.set_type (std::move (tc));
	}
}

}
}
