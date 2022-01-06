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
#include <CORBA/CORBA.h>

namespace CORBA {

namespace Internal {

void Type <Any>::check (const ABI& any)
{
	TypeCode::_check (any.type ());
	if (any.is_large () && !any.large_pointer ())
		::Nirvana::throw_BAD_PARAM ();
}

void Type <Any>::marshal_in (const Any& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	I_ptr <TypeCode> tc = src.type ();
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

void Type <Any>::marshal_out (Any& src, Marshal_ptr marshaler, ABI& dst)
{
	assert (&src != &dst);
	I_ptr <TypeCode> tc = src.type ();
	if (!tc)
		dst.reset ();
	else {
		dst.type ((::Nirvana::Pointer)marshaler->marshal_interface (tc));
		::Nirvana::Pointer psrc;
		::Nirvana::Pointer pdst;
		if (src.is_large ()) {
			psrc = src.large_pointer ();
			size_t size = tc->_size ();
			if (!tc->_has_marshal ()) {
				uintptr_t p = marshaler->marshal_memory (psrc, size, src.large_size ());
				dst.large_pointer ((void*)p, size);
				src.reset ();
				return;
			} else {
				uintptr_t p = marshaler->get_buffer (size, pdst);
				dst.large_pointer ((void*)p, size);
			}
		} else {
			pdst = dst.small_pointer ();
			psrc = src.small_pointer ();
		}
		tc->_marshal_out (psrc, marshaler, pdst);
		src.clear ();
	}
}

void Type <Any>::unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Any& dst)
{
	TypeCode* ptc = src.type ();
	if (!ptc)
		dst.reset ();
	else {
		I_ref <TypeCode> tc (unmarshaler->unmarshal_interface <TypeCode> (ptc));
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

using namespace Internal;

void Any::clear ()
{
	I_ptr <TypeCode> tc = type ();
	bool large = is_large ();
	if (tc) {
		void* p;
		if (large)
			p = large_pointer ();
		else
			p = small_pointer ();
		tc->_destruct (p);
		interface_release (&tc);
	}
	if (large)
		::Nirvana::g_memory->release (large_pointer (), large_size ());
	reset ();
}

void* Any::prepare (I_ptr <TypeCode> tc)
{
	clear ();
	void* dst = nullptr;
	if (tc) {
		size_t size = tc->_size ();
		if (!size)
			::Nirvana::throw_BAD_TYPECODE ();
		if (size <= SMALL_CAPACITY)
			dst = small_pointer ();
		else
			large_pointer (dst = ::Nirvana::g_memory->allocate (0, size, 0), size);
	}
	return dst;
}

void Any::set_type (I_ptr <TypeCode> tc)
{
	ABI::type (interface_duplicate (&tc));
}

void Any::set_type (I_ref <TypeCode>&& tc)
{
	I_ptr <TypeCode> tcp (nullptr);
	reinterpret_cast <I_ref <TypeCode>&> (tcp) = std::move (tc);
	ABI::type (&tcp);
}

void Any::copy_from (I_ptr <TypeCode> tc, const void* val)
{
	void* dst = prepare (tc);
	if (dst) {
		tc->_copy (dst, val);
		set_type (tc);
	}
}

void Any::copy_from (const Any& src)
{
	I_ptr <TypeCode> tc = src.type ();
	void* dst = prepare (tc);
	if (dst) {
		tc->_copy (dst, src.data ());
		set_type (tc);
	}
}

void Any::move_from (I_ptr <TypeCode> tc, void* val)
{
	void* dst = prepare (tc);
	tc->_move (dst, val);
	set_type (tc);
}

void Any::type (I_ptr <TypeCode> alias)
{
	I_ptr <TypeCode> tc = type ();
	if (tc && tc->equivalent (alias)) {
		interface_release (&tc);
		set_type (alias);
	}
}

static_assert (Internal::ABI <Any>::SMALL_CAPACITY >= sizeof (SystemException::_Data), "Any data must fit SystemException::_Data.");

void* Any::data ()
{
	assert (type ());
	if (is_large ())
		return large_pointer ();
	else
		return small_pointer ();
}

void Any::operator <<= (from_boolean from)
{
	clear ();
	*(Boolean*)small_pointer () = from.val;
	set_type (_tc_boolean);
}

Boolean Any::operator >>= (to_boolean to) const
{
	if (type ()->equivalent (_tc_boolean)) {
		to.ref = *(const Boolean*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_char from)
{
	clear ();
	*(Char*)small_pointer () = from.val;
	set_type (_tc_char);
}

Boolean Any::operator >>= (to_char to) const
{
	if (type ()->equivalent (_tc_char)) {
		to.ref = *(const Char*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_wchar from)
{
	clear ();
	*(WChar*)small_pointer () = from.val;
	set_type (_tc_wchar);
}

Boolean Any::operator >>= (to_wchar to) const
{
	if (type ()->equivalent (_tc_wchar)) {
		to.ref = *(const WChar*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_octet from)
{
	clear ();
	*(Octet*)small_pointer () = from.val;
	set_type (_tc_octet);
}

Boolean Any::operator >>= (to_octet to) const
{
	if (type ()->equivalent (_tc_octet)) {
		to.ref = *(const Octet*)small_pointer ();
		return true;
	} else
		return false;
}

void operator <<= (Any& any, const Exception& e)
{
	any.copy_from (e.__type_code (), e.__data ());
}

void operator <<= (Any& any, Exception&& e)
{
	any.move_from (e.__type_code (), e.__data ());
}

Boolean operator >>= (const Any& any, SystemException& se)
{
	I_ptr <TypeCode> tc = any.type ();
	if (tc && tc->kind () == tk_except) {
		const Char* id = tc->id ().c_str ();
		const Char standard_prefix [] = "IDL:omg.org/CORBA/";
		const Char nirvana_prefix [] = "IDL:CORBA/";
		if (
			!strncmp(standard_prefix, id, countof (standard_prefix) - 1)
		||
			!strncmp (nirvana_prefix, id, countof (nirvana_prefix) - 1)
		) {
			const Internal::ExceptionEntry* ee = SystemException::_get_exception_entry (id, Exception::EC_SYSTEM_EXCEPTION);
			assert (ee);
			(ee->construct) (&se);
			const SystemException::_Data& data = *(const SystemException::_Data*)any.data ();
			se.completed (data.completed);
			if (RepositoryId::compatible (ee->rep_id, id))
				se.minor (data.minor);
			return true;
		}
	}
	return false;
}

}
