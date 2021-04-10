/// \file MarshalContext.h
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
#ifndef NIRVANA_ORB_MARSHALCONTEXT_H_
#define NIRVANA_ORB_MARSHALCONTEXT_H_

#include <CORBA/CORBA.h>
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

/// \brief Type of the inter-domain marshal context.
enum class MarshalContext : ABI_enum
{
	SHARED_MEMORY,            ///< Both domains share common memory heap.
	SHARED_PROTECTION_DOMAIN, ///< Different heaps in the common protection domain.
	OTHER_PROTECTION_DOMAIN   ///< Different protection domains.
};

template <> struct Type <MarshalContext> : 
	TypeEnum <MarshalContext, MarshalContext::OTHER_PROTECTION_DOMAIN>
{};

}
}

#endif
