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
#include "../../pch/pch.h"
#include <Nirvana/DecCalc.h>

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
	I_ref <TypeCode> tc;
	rq->unmarshal_type_code (tc);
	unmarshal (tc, rq, dst);
}

void Type <Any>::unmarshal (I_ptr <TypeCode> tc, IORequest_ptr rq, Any& dst)
{
	if (tc && tc->kind () != TCKind::tk_void) {
		void* p = dst.prepare (tc);
		tc->n_construct (p);
		dst.set_type (tc);
		tc->n_unmarshal (rq, 1, p);
	}
}

}

using namespace Internal;

Any::~Any ()
{
	try {
		clear ();
	} catch (...) {
		// TODO: Log
		assert (false);
	}
}

void Any::clear ()
{
	I_ptr <TypeCode> tc = type ();
	if (tc) {
		bool large = is_large ();
		void* p;
		if (large)
			p = large_pointer ();
		else
			p = small_pointer ();
		tc->n_destruct (p);
		interface_release (&tc);
		reset ();
		if (large)
			::Nirvana::the_memory->release (p, large_size ());
	}
}

void* Any::prepare (I_ptr <TypeCode> tc)
{
	clear ();
	void* dst = nullptr;
	if (tc) {
		size_t size = tc->n_size ();
		if (size <= SMALL_CAPACITY)
			dst = small_pointer ();
		else {
			dst = ::Nirvana::the_memory->allocate (0, size, 0);
			large_pointer (dst, size);
		}
	}
	return dst;
}

void Any::set_type (I_ptr <TypeCode> tc)
{
	ABI::type (interface_duplicate (&tc));
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

void Any::construct (Internal::I_ptr <TypeCode> tc)
{
	tc->n_construct (prepare (tc));
}

void* Any::data () noexcept
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
	TypeCode::_ptr_type tc = e.__type_code ();
	if (tc->equal (_tc_UnknownUserException))
		any = *(const Any*)e.__data ();
	else
		any.copy_from (tc, e.__data ());
}

void operator <<= (Any& any, Exception&& e)
{
	TypeCode::_ptr_type tc = e.__type_code ();
	if (tc->equal (_tc_UnknownUserException))
		any = std::move (*(Any*)e.__data ());
	else
		any.move_from (tc, e.__data ());
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

void Any::operator <<= (from_fixed ff)
{
	TypeCode::_ref_type tc = the_orb->create_fixed_tc (ff.digits, ff.scale);
	void* p = prepare (tc);
	Nirvana::dec_calc->to_BCD (ff.val, ff.digits, ff.scale, (Octet*)p);
	set_type (tc);
}

Boolean Any::operator >>= (to_fixed tf) const
{
	if (type () && type ()->kind () == TCKind::tk_fixed) {
		UShort digits = type ()->fixed_digits ();
		Short scale = type ()->fixed_scale ();
		if (digits - scale <= tf.digits - tf.scale) {
			Nirvana::DecCalc::Number n;
			Nirvana::dec_calc->from_BCD (n, digits, scale, (const Octet*)data ());
			if (scale > tf.scale)
				Nirvana::dec_calc->round (n, tf.scale);
			tf.val = reinterpret_cast <const Fixed&> (n);
			return true;
		}
	}
	return false;
}

void Any::operator <<= (from_string fs)
{
	TypeCode::_ref_type tc;
	if (fs.bound)
		tc = the_orb->create_string_tc (fs.bound);
	else
		tc = _tc_string;
	Internal::String_var <Char> sv;
	if (fs.nocopy)
		sv = fs.val; // Adopt string
	else
		sv = (const Char*)fs.val;
	move_from (tc, &sv);
}

void Any::operator <<= (from_wstring fs)
{
	TypeCode::_ref_type tc;
	if (fs.bound)
		tc = the_orb->create_wstring_tc (fs.bound);
	else
		tc = _tc_wstring;
	Internal::String_var <WChar> sv;
	if (fs.nocopy)
		sv = fs.val; // Adopt string
	else
		sv = (const WChar*)fs.val;
	move_from (tc, &sv);
}

Boolean Any::operator >>= (to_string ts) const
{
	if (type ()->kind () == TCKind::tk_string) {
		const Internal::String& s = *(const Internal::String*)data ();
		if (!ts.bound || s.length () <= (size_t)ts.bound) {
			ts.val = s.c_str ();
			return true;
		}
	}
	return false;
}

Boolean Any::operator >>= (to_wstring ts) const
{
	if (type ()->kind () == TCKind::tk_wstring) {
		const Internal::WString& s = *(const Internal::WString*)data ();
		if (!ts.bound || s.length () <= (size_t)ts.bound) {
			ts.val = s.c_str ();
			return true;
		}
	}
	return false;
}

}
