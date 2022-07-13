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
	TypeCode::_ref_type tc = g_ORB->create_fixed_tc (ff.digits, ff.scale);
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

}
