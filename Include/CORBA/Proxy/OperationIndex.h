/// \file
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
#ifndef NIRVANA_ORB_OPERATIONINDEX_H_
#define NIRVANA_ORB_OPERATIONINDEX_H_

#include "../primitive_types.h"

namespace CORBA {
namespace Internal {

typedef ULong OperationIndex;

inline OperationIndex make_op_idx (UShort interface_idx, UShort op_idx) noexcept
{
	return ((ULong)interface_idx << 16) | op_idx;
}

inline UShort interface_idx (OperationIndex oi) noexcept
{
	return oi >> 16;
}

inline UShort operation_idx (OperationIndex oi) noexcept
{
	return (UShort)oi;
}

}
}

#endif
