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
#ifndef NIRVANA_ORB_TC_CONSTANTS_H_
#define NIRVANA_ORB_TC_CONSTANTS_H_
#pragma once

#include <Nirvana/ImportInterface.h>

namespace CORBA {

#define _tc_null TypeCode::_nil ()
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_void;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_short;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_long;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_longlong;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_ushort;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_ulong;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_ulonglong;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_float;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_double;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_longdouble;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_boolean;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_char;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_wchar;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_octet;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_any;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_string;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_wstring;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_Object;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_TypeCode;
extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <TypeCode> _tc_ValueBase;

}

#endif
