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
#ifndef NIRVANA_ORB_LIFECYCLEDYNAMIC_H_
#define NIRVANA_ORB_LIFECYCLEDYNAMIC_H_
#pragma once

#include "Exception.h"

namespace CORBA {
namespace Internal {

//! Dynamic servant life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
template <class S>
class LifeCycleDynamic
{
public:
	template <class Itf>
	static Interface* __duplicate (Interface* itf, Interface* env) noexcept
	{
		try {
			return S::_duplicate (static_cast <Bridge <Itf>*> (itf));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	template <class Itf>
	static void __release (Interface* itf) noexcept
	{
		try {
			S::_release (static_cast <Bridge <Itf>*> (itf));
		} catch (...) {
		}
	}
};

}
}

#endif
