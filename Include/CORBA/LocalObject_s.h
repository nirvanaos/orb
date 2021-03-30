/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_LOCALOBJECT_S_H_
#define NIRVANA_ORB_LOCALOBJECT_S_H_

#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, LocalObject>
{
public:
	static const typename Bridge <LocalObject>::EPV epv_;

protected:
	static ABI_boolean __non_existent (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <LocalObject>::EPV Skeleton <S, LocalObject>::epv_ = {
	{ // interface
		Bridge <LocalObject>::repository_id_,
		S::template __duplicate <LocalObject>,
		S::template __release <LocalObject>
	},
	{ // base
		S::template _wide_object <LocalObject>,
		S::template _wide <ReferenceCounter, LocalObject>
	},
	{ // epv
		S::__non_existent
	}
};

}
}

#endif
