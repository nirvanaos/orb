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
		check_pointer (bridge, Skeleton <ServantPOA <I>, I>::epv_.header);
		return static_cast <ServantPOA <I>&> (*bridge);
	}

	template <class I>
	static const ServantPOA <I>& _implementation (const Bridge <I>* bridge)
	{
		return _implementation (const_cast <Bridge <I>*> (bridge));
	}

};

}
}

#endif
