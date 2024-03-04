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

// Methods that required DecCalc module are moved to CoreImports.
// These methods can not be called from Core.

#include <CORBA/CORBA.h>

using namespace Nirvana;

namespace CORBA {

void Any::operator <<= (from_fixed ff)
{
	TypeCode::_ref_type tc = orb->create_fixed_tc (ff.digits, ff.scale);
	void* p = prepare (tc);
	g_dec_calc->to_BCD (ff.val, ff.digits, ff.scale, (Octet*)p);
	set_type (tc);
}

Boolean Any::operator >>= (to_fixed tf) const
{
	if (type () && type ()->kind () == TCKind::tk_fixed) {
		UShort digits = type ()->fixed_digits ();
		Short scale = type ()->fixed_scale ();
		if (digits - scale <= tf.digits - tf.scale) {
			DecCalc::Number n;
			g_dec_calc->from_BCD (n, digits, scale, (const Octet*)data ());
			if (scale > tf.scale)
				g_dec_calc->round (n, tf.scale);
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
		tc = orb->create_string_tc (fs.bound);
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
		tc = orb->create_wstring_tc (fs.bound);
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
