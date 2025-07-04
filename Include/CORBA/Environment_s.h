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
#ifndef NIRVANA_ORB_ENVIRONMENT_S_H_
#define NIRVANA_ORB_ENVIRONMENT_S_H_
#pragma once

#include "Environment_c.h"

namespace CORBA {
namespace Internal {

template <class S, class Itf> class Skeleton;

template <class S>
class Skeleton <S, ::CORBA::Environment>
{
public:
	static const typename Bridge < ::CORBA::Environment>::EPV epv_;

protected:
	static void _s_exception_set (Bridge < ::CORBA::Environment>* obj, Short code, const char* rep_id, void* param)
	{
		try {
			S::_implementation (obj).exception_set (code, rep_id, param);
		} catch (...) {
		}
	}

	static const Char* _s_exception_id (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_id ();
		} catch (...) {
		}
		return 0;
	}

	static const void* _s_exception_value (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_value ();
		} catch (...) {
		}
		return 0;
	}

	static void _s_exception_free (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			S::_implementation (obj).exception_free ();
		} catch (...) {
		}
	}
};

template <class S>
const Bridge < ::CORBA::Environment>::EPV Skeleton <S, ::CORBA::Environment>::epv_ = {
	{ // header
		RepIdOf < ::CORBA::Environment>::id,
		S::template __duplicate < ::CORBA::Environment>,
		S::template __release < ::CORBA::Environment>
	},
	{ // epv
		S::_s_exception_set,
		S::_s_exception_id,
		S::_s_exception_value,
		S::_s_exception_free
	}
};

}
}

#endif
