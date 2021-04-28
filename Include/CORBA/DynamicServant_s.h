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
#ifndef NIRVANA_ORB_DYNAMICSERVANT_S_H_
#define NIRVANA_ORB_DYNAMICSERVANT_S_H_

#include "DynamicServant.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, DynamicServant>
{
public:
	static const typename Bridge <DynamicServant>::EPV epv_;

	void _delete ()
	{
		delete &static_cast <S&> (*this);
	}

protected:
	static void __delete (Bridge <DynamicServant>* obj, Interface* env)
	{
		try {
			S::_implementation (obj)._delete ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}
};

template <class S>
const Bridge <DynamicServant>::EPV Skeleton <S, DynamicServant>::epv_ = {
	{ // header
		Bridge <DynamicServant>::repository_id_,
		S::template __duplicate <DynamicServant>,
		S::template __release <DynamicServant>
	},
	{ // epv
		S::__delete
	}
};

}
}

#endif
