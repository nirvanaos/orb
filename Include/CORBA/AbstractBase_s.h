/// \file AbstractBase_s.h
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
#ifndef NIRVANA_ORB_ABSTRACTBASE_S_H_
#define NIRVANA_ORB_ABSTRACTBASE_S_H_
#pragma once

#include "AbstractBase.h"

// Servant part of an interface

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, AbstractBase>
{
public:
	typedef S ServantType;
	static const typename Bridge <AbstractBase>::EPV epv_;

protected:
	static Interface* __to_object (Bridge <AbstractBase>* _b, Interface* _env)
	{
		try {
			return Type <Object>::ret (S::_implementation (_b)._to_object ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Interface>::ret ();
	}

	static Interface* __to_value (Bridge <AbstractBase>* _b, Interface* _env)
	{
		try {
			return Type <ValueBase>::ret (S::_implementation (_b)._to_value ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Interface>::ret ();
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::epv_ = {
	{	// header
		RepIdOf <AbstractBase>::id,
		S::template __duplicate <AbstractBase>,
		S::template __release <AbstractBase>
	},
	{	// epv
		S::__to_object,
		S::__to_value
	}
};

}
}

#endif
