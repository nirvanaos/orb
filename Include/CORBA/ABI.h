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
#ifndef NIRVANA_ORB_ABI_H_
#define NIRVANA_ORB_ABI_H_

namespace CORBA {
namespace Nirvana {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.
/// So we use size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
/// Note that vector <bool> template specialization has element size is 1 byte.
typedef size_t ABI_boolean;

/// For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::ABI <T>` structure.
/// ABI type must be POD (Plain Old Data, mustn't have any constructors and destructors).
/// Compiler replaces all non-POD struct members with corresponding ABI structures.
template <class T> struct ABI;

}
}

#endif
