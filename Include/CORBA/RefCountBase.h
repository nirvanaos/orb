/// \file
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
#ifndef NIRVANA_ORB_REFCOUNTBASE_H_
#define NIRVANA_ORB_REFCOUNTBASE_H_
#pragma once

#include "RefCountLink.h"

namespace CORBA {
namespace Internal {

template <class T>
class RefCountBase :
	public RefCountLink
{
private:
	static void _s_delete_object (Bridge <DynamicServant>* _b, Interface* _env)
	{
		try {
			check_pointer (_b, epv_.header);
			T* p = &static_cast <T&> (reinterpret_cast <RefCountBase <T>&> (*_b));
			delete p;
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

protected:
	RefCountBase () :
		RefCountLink (epv_)
	{}

private:
	static const Bridge <DynamicServant>::EPV epv_;
};

template <class T>
const Bridge <DynamicServant>::EPV RefCountBase <T>::epv_ = {
	{ // header
		RepIdOf <DynamicServant>::id,
		RefCountLink::__dup,
		RefCountLink::__rel
	},
	{ // EPV
		_s_delete_object
	}
};

}
}

#endif
