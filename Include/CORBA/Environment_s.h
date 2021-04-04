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

#include "Environment_c.h"

namespace CORBA {
namespace Nirvana {

template <class S, class I> class Skeleton;

template <class S>
class Skeleton <S, ::CORBA::Environment>
{
public:
	static const typename Bridge < ::CORBA::Environment>::EPV epv_;

protected:
	static void _exception_set (Bridge < ::CORBA::Environment>* obj, Short code, const char* rep_id, const void* param)
	{
		try {
			S::_implementation (obj).exception_set (code, rep_id, param);
		} catch (...) {
		}
	}

	static const Char* _exception_id (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_id ();
		} catch (...) {
		}
		return 0;
	}

	static const void* _exception_value (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_value ();
		} catch (...) {
		}
		return 0;
	}

	static void _exception_free (Bridge < ::CORBA::Environment>* obj)
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
		Bridge < ::CORBA::Environment>::repository_id_,
		S::template __duplicate < ::CORBA::Environment>,
		S::template __release < ::CORBA::Environment>
	},
	{ // epv
		S::_exception_set,
		S::_exception_id,
		S::_exception_value,
		S::_exception_free
	}
};

}
}

#endif
