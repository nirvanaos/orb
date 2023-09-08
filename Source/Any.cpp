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
#include "pch.h"

namespace CORBA {

namespace Internal {

void Type <Any>::check (const ABI& any)
{
	TypeCode::_check (any.type ());
	if (any.is_large () && !any.large_pointer ())
		::Nirvana::throw_BAD_PARAM ();
}

void Type <Any>::marshal_in (const Any& src, IORequest_ptr rq)
{
	I_ptr <TypeCode> tc = src.type ();
	rq->marshal_type_code (tc);
	if (tc)
		tc->n_marshal_in (src.data (), 1, rq);
}

void Type <Any>::marshal_out (Any& src, IORequest_ptr rq)
{
	I_ptr <TypeCode> tc = src.type ();
	rq->marshal_type_code (tc);
	if (tc) {
		tc->n_marshal_out (src.data (), 1, rq);
		src.clear ();
	}
}

void Type <Any>::unmarshal (IORequest_ptr rq, Any& dst)
{
	I_ref <TypeCode> tc = rq->unmarshal_type_code ();
	unmarshal (tc, rq, dst);
}

void Type <Any>::unmarshal (I_ptr <TypeCode> tc, IORequest_ptr rq, Any& dst)
{
	if (tc && tc->kind () != TCKind::tk_void) {
		void* p = dst.prepare (tc);
		tc->n_construct (p);
		tc->n_unmarshal (rq, 1, p);
		dst.set_type (tc);
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
		tc->n_destruct (p);
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
		size_t size = tc->n_aligned_size ();
		if (size <= SMALL_CAPACITY)
			dst = small_pointer ();
		else {
			dst = ::Nirvana::g_memory->allocate (0, size, 0);
			large_pointer (dst, size);
		}
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
		tc->n_copy (dst, val);
		set_type (tc);
	}
}

void Any::copy_from (const Any& src)
{
	I_ptr <TypeCode> tc = src.type ();
	void* dst = prepare (tc);
	if (dst) {
		tc->n_copy (dst, src.data ());
		set_type (tc);
	}
}

void Any::move_from (I_ptr <TypeCode> tc, void* val)
{
	void* dst = prepare (tc);
	tc->n_move (dst, val);
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
	*(Type <Boolean>::ABI*)small_pointer () = from.val;
	set_type (_tc_boolean);
}

Boolean Any::operator >>= (to_boolean to) const
{
	if (type ()->equivalent (_tc_boolean)) {
		to.ref = *(const Type <Boolean>::ABI*)small_pointer ();
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

bool Any::is_system_exception () const noexcept
{
	I_ptr <TypeCode> tc = type ();
	if (tc && tc->kind () == TCKind::tk_except) {
		std::string id = tc->id ();
		const Char standard_prefix [] = "IDL:omg.org/CORBA/";
		const Char nirvana_prefix [] = "IDL:CORBA/";
		return
			!strncmp (standard_prefix, id.c_str (), countof (standard_prefix) - 1)
			||
			!strncmp (nirvana_prefix, id.c_str (), countof (nirvana_prefix) - 1);
	}
	return false;
}

Boolean operator >>= (const Any& any, SystemException& se)
{
	if (any.is_system_exception ()) {
		I_ptr <TypeCode> tc = any.type ();
		std::string id = tc->id ();
		const Internal::ExceptionEntry* ee = SystemException::_get_exception_entry (id, Exception::EC_SYSTEM_EXCEPTION);
		if (ee) {
			(ee->construct) (&se);
			const SystemException::_Data& data = *(const SystemException::_Data*)any.data ();
			se.completed (data.completed);
			se.minor (data.minor);
		} else
			new (&se) UNKNOWN (MAKE_OMG_MINOR (2)); // Non-standard System Exception not supported.
		return true;
	}
	return false;
}

void Any::construct (Internal::I_ptr <TypeCode> tc)
{
	tc->n_construct (prepare (tc));
}

}
