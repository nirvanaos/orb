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
#ifndef NIRVANA_ORB_PSEUDOBASE_S_H_
#define NIRVANA_ORB_PSEUDOBASE_S_H_
#pragma once

#include "PseudoBase.h"

// Servant part of an interface

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, PseudoBase>
{
public:
	typedef S ServantType;
	static const typename Bridge <PseudoBase>::EPV epv_;

protected:
	static Interface* __query_interface (Bridge <PseudoBase>* base, Type <String>::ABI_in id, Interface* env)
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
};

template <class S>
const Bridge <PseudoBase>::EPV Skeleton <S, PseudoBase>::epv_ = {
	{	// header
		Bridge <PseudoBase>::repository_id_,
		&S::template __duplicate <PseudoBase>,
		&S::template __release <PseudoBase>
	},
	{	// epv
		&S::__query_interface
	}
};

}
}

#endif
