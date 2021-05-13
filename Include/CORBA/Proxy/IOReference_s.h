/// \file IOReference_s.h
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
#ifndef NIRVANA_ORB_IOREFERENCE_S_H_
#define NIRVANA_ORB_IOREFERENCE_S_H_

#include "IOReference.h"

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, IOReference>
{
public:
	static const typename Bridge <IOReference>::EPV epv_;

protected:
	static Interface* __get_object (Bridge <IOReference>* _b, Interface* _env)
	{
		try {
			return TypeItf <Object>::VT_ret (S::_implementation (_b).object ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _create_marshaler (Bridge <IOReference>* _b, Interface* _env)
	{
		try {
			return TypeItf <Marshal>::ret (S::_implementation (_b).create_marshaler ());
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _call (Bridge <IOReference>* _b, Type <OperationIndex>::ABI_in op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Interface** marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size, Interface* _env)
	{
		try {
			return Type <Unmarshal>::ret (S::_implementation (_b).call (Type <OperationIndex>::in (op),
				in_params, in_params_size, Type <Marshal>::inout (marshaler), out_params, out_params_size));
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <IOReference>::EPV Skeleton <S, IOReference>::epv_ = {
	{ // header
		Bridge <IOReference>::repository_id_,
		S::template __duplicate <IOReference>,
		S::template __release <IOReference>
	},
	{ // epv
		S::__get_object,
		S::_create_marshaler,
		S::_call
	}
};

}
}

#endif
