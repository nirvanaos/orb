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
#ifndef NIRVANA_ORB_VALUEBASE_S_H_
#define NIRVANA_ORB_VALUEBASE_S_H_
#pragma once

#include "ValueBase.h"

namespace CORBA {
namespace Internal {

// ValueBase skeleton

template <class S>
class Skeleton <S, ValueBase>
{
public:
	static const typename Bridge <ValueBase>::EPV epv_;

protected:
	static Interface* __copy_value (Bridge <ValueBase>* _b, Interface* _env)
	{
		try {
			return Type <ValueBase>::ret (S::_implementation (_b)._copy_value ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <ValueBase>::ret ();
	}

	static void __marshal (Bridge <ValueBase>* _b, Interface* rq, Interface* _env)
	{
		try {
			S::_implementation (_b)._marshal (Type <IORequest>::in (rq));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __unmarshal (Bridge <ValueBase>* _b, Interface* rq, Interface* _env)
	{
		try {
			S::_implementation (_b)._unmarshal (Type <IORequest>::in (rq));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Interface* __query_valuetype (Bridge <ValueBase>* _b, Type <String>::ABI_in id, Interface* _env)
	{
		try {
			return Type <Interface>::VT_ret (S::_implementation (_b)._query_valuetype (Type <String>::in (id)));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Interface>::VT_ret ();
	}

	static Interface* __truncatable_base (Bridge <ValueBase>* _b, Interface* _env)
	{
		try {
			return Type <TypeCode>::C_ret (S::_implementation (_b)._truncatable_base ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <TypeCode>::C_ret ();
	}

};

template <class S>
const Bridge <ValueBase>::EPV Skeleton <S, ValueBase>::epv_ = {
	{ // header
		RepIdOf <ValueBase>::id,
		S::template __duplicate <ValueBase>,
		S::template __release <ValueBase>
	},
	{ // epv
		S::__copy_value,
		S::__marshal,
		S::__unmarshal,
		S::__query_valuetype,
		S::__truncatable_base
	}
};

}
}

#endif
