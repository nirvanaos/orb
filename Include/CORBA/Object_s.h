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
#ifndef NIRVANA_ORB_OBJECT_S_H_
#define NIRVANA_ORB_OBJECT_S_H_
#pragma once

#include "Object.h"

namespace CORBA {
namespace Internal {

//! Object skeleton

template <class S>
class Skeleton <S, Object>
{
public:
	static const typename Bridge <Object>::EPV epv_;

protected:
	static Interface* __get_interface (Bridge <Object>* obj, Interface* env)
	{
		try {
			return Type <InterfaceDef>::ret (S::_implementation (obj)._get_interface ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret __is_a (Bridge <Object>* obj, Type <String>::ABI_in type_id, Interface* env)
	{
		try {
			return S::_implementation (obj)._is_a (Type <String>::in (type_id));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret __non_existent (Bridge <Object>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret __is_equivalent (Bridge <Object>* obj, Interface* other_object, Interface* env)
	{
		try {
			return S::_implementation (obj)._is_equivalent (Type <Object>::in (other_object));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static ULong __hash (Bridge <Object>* obj, ULong maximum, Interface* env)
	{
		try {
			return S::_implementation (obj)._hash (maximum);
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
	// TODO: Other Object operations shall be here...

	// Internals
	static Interface* __query_interface (Bridge <Object>* base, Type <String>::ABI_in id, Interface* env)
	{
		try {
			return Type <Interface>::VT_ret (S::_implementation (base)._query_interface (Type <String>::in (id)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Interface>::VT_ret ();
	}

	static Interface* __get_servant (Bridge <Object>* obj, Interface* env)
	{
		set_MARSHAL (env);
		return 0;
	}
};

template <class S>
const Bridge <Object>::EPV Skeleton <S, Object>::epv_ = {
	{ // header
		RepIdOf <Object>::id,
		S::template __duplicate <Object>,
		S::template __release <Object>
	},
	{ // epv
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::__is_equivalent,
		S::__hash,
		// TODO: Other Object operations shall be here...
		
		S::__query_interface
	}
};

}
}

#endif
