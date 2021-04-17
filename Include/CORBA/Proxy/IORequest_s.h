/// \file IORequest_s.h
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
#ifndef NIRVANA_ORB_IOREQUEST_S_H_
#define NIRVANA_ORB_IOREQUEST_S_H_

#include "IORequest.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, IORequest>
{
public:
	static const typename Bridge <IORequest>::EPV epv_;

protected:
	static Interface* _get_marshaler (Bridge <IORequest>* _b, Interface* _env)
	{
		try {
			return Type <Marshal>::ret (S::_implementation (_b).marshaler ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static void _exception (Bridge <IORequest>* _b, Type <Any>::ABI_out exc)
	{
		try {
			S::_implementation (_b).exception (Type <Any>::inout (exc));
		} catch (...) {
			// Swallow exception. TODO: Log.
		}
	}

	static void _success (Bridge <IORequest>* _b, Interface* _env)
	{
		try {
			S::_implementation (_b).success ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge <IORequest>::EPV Skeleton <S, IORequest>::epv_ = {
	{ // header
		Bridge <IORequest>::repository_id_,
		S::template __duplicate <IORequest>,
		S::template __release <IORequest>
	},
	{ // epv
		S::_get_marshaler,
		S::_exception,
		S::_success
	}
};

}
}

#endif
