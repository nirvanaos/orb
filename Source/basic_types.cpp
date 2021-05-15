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

#define DEF_BASIC_TYPE(T, tc) I_ptr <TypeCode> Internal::Type <T>::type_code () { return tc; }

namespace CORBA {
namespace Internal {

DEF_BASIC_TYPE (void, _tc_void)
DEF_BASIC_TYPE (Char, _tc_char)
DEF_BASIC_TYPE (WChar, _tc_wchar)
DEF_BASIC_TYPE (Octet, _tc_octet)
DEF_BASIC_TYPE (Short, _tc_short)
DEF_BASIC_TYPE (UShort, _tc_ushort)
DEF_BASIC_TYPE (Long, _tc_long)
DEF_BASIC_TYPE (LongLong, _tc_longlong)
DEF_BASIC_TYPE (ULong, _tc_ulong)
DEF_BASIC_TYPE (ULongLong, _tc_ulonglong)
DEF_BASIC_TYPE (Float, _tc_float)
DEF_BASIC_TYPE (Double, _tc_double)
DEF_BASIC_TYPE (LongDouble, _tc_longdouble)
DEF_BASIC_TYPE (Boolean, _tc_boolean)
template <> DEF_BASIC_TYPE (StringT <Char>, _tc_string)
template <> DEF_BASIC_TYPE (StringT <WChar>, _tc_wstring)
DEF_BASIC_TYPE (Any, _tc_any)
DEF_BASIC_TYPE (Object, _tc_Object)

}
}
