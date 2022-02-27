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
#ifndef NIRVANA_ORB_LOCALOBJECT_S_H_
#define NIRVANA_ORB_LOCALOBJECT_S_H_
#pragma once

#include "LocalObject.h"

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, LocalObject>
{
public:
	static const typename Bridge <LocalObject>::EPV epv_;

protected:
	static Type <Boolean>::ABI_ret __non_existent (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static void __add_ref (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			S::_implementation (obj)._add_ref ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void __remove_ref (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			S::_implementation (obj)._remove_ref ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static ULong __refcount_value (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._refcount_value ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static void ___delete_object (Bridge <LocalObject>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).__delete_object ();
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
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
		S::template _wide_object <LocalObject>
	},
	{ // epv
		S::__non_existent,
		S::__add_ref,
		S::__remove_ref,
		S::__refcount_value,
		S::___delete_object
	}
};

}
}

#endif
