/// \file
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
#ifndef NIRVANA_ORB_VALUEFACTORYBASE_S_H_
#define NIRVANA_ORB_VALUEFACTORYBASE_S_H_
#pragma once

#include "ValueFactoryBase.h"

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, ValueFactoryBase>
{
public:
	static const typename Bridge <ValueFactoryBase>::EPV epv_;

protected:
	static Interface* _s_create_for_unmarshal (Bridge <ValueFactoryBase>* _b, Interface* _env)
	{
		try {
			return Type <ValueBase>::ret (S::_implementation (_b).create_for_unmarshal ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <ValueBase>::ret ();
	}

	static Interface* __query_factory (Bridge <ValueFactoryBase>* _b, Type <String>::ABI_in id, Interface* _env)
	{
		try {
			return Type <Interface>::VT_ret (S::_implementation (_b)._query_factory (Type <String>::in (id)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Interface>::VT_ret ();
	}

};

template <class S>
const Bridge <ValueFactoryBase>::EPV Skeleton <S, ValueFactoryBase>::epv_ = {
	{ // header
		RepIdOf <ValueFactoryBase>::id,
		S::template __duplicate <ValueFactoryBase>,
		S::template __release <ValueFactoryBase>
	},
	{ // epv
		S::_s_create_for_unmarshal,
		S::__query_factory
	}
};

}
}

#endif
