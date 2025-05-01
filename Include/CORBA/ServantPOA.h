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

template <class Itf> class ServantPOA;

class ServantTraitsPOA :
	public ServantTraits <ServantTraitsPOA>
{
public:
	template <class Itf>
	static ServantPOA <Itf>& _implementation (Bridge <Itf>* bridge)
	{
		check_pointer (bridge, Skeleton <ServantPOA <Itf>, Itf>::epv_.header);
		return static_cast <ServantPOA <Itf>&> (*bridge);
	}

	template <class Itf>
	static const ServantPOA <Itf>& _implementation (const Bridge <Itf>* bridge)
	{
		return _implementation (const_cast <Bridge <Itf>*> (bridge));
	}

};

}
}

#endif
