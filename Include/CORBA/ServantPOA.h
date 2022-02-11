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
#ifndef NIRVANA_ORB_SERVANTPOA_H_
#define NIRVANA_ORB_SERVANTPOA_H_
#pragma once

#include "ServantImpl.h"

namespace CORBA {
namespace Internal {

template <class I> class ServantPOA;

class ServantTraitsPOA :
	public ServantTraits <ServantTraitsPOA>
{
public:
	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <I>, I>::epv_.header);
#ifdef _DEBUG
		ServantPOA <I>* impl = static_cast <ServantPOA <I>*> (bridge);
		// Bridge must be first base of the servant to reduce the code overhead:
		assert ((uintptr_t)bridge - (uintptr_t)impl == sizeof (void*));
		return *impl;
#else
		return static_cast <ServantPOA <I>&> (*bridge);
#endif
	}
};

}
}

#endif
