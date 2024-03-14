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
#ifndef NIRVANA_ORB_TYPEVARLEN_H_
#define NIRVANA_ORB_TYPEVARLEN_H_
#pragma once

#include "TypeByRef.h"
#include "MarshalHelper.h"
#include <utility>
#include <new>

namespace CORBA {
namespace Internal {

/// Base for variable-length data types
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <class T, class TABI = ABI <T> >
struct TypeVarLen :
	TypeByRefCheck <T, TABI>,
	MarshalHelper <T, typename TypeByRef <T, TABI>::Var>
{
	static const bool is_var_len = true;
	static const bool is_CDR = false;
	static const bool is_CDR_seq = false;
	static const bool has_check = true;

	typedef TypeByRef <T, TABI> Base;
	typedef typename Base::Var Var;
	typedef typename Base::C_in C_in;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_ret ABI_ret;
	typedef typename Base::ABI ABI;

	// C_out class clears output variable
	class C_out : public Base::C_out
	{
	public:
		C_out (Var& val) :
			Base::C_out (val)
		{
			val = Var ();	// Clear
		}
	};

	// Servant-side methods

	// `const` is removed to let servant adopt the unmarshaled input data.
	static Var& in (typename Base::ABI_in p)
	{
		return const_cast <Var&> (Base::in (p));
	}

	static ABI_ret ret (Var&& v) noexcept
	{
		ABI_ret abi;
		new (&abi) Var (std::move (v));
		return abi;
	}

	static ABI_ret ret (const Var& v)
	{
		ABI_ret abi;
		new (&abi) Var (v);
		return abi;
	}

	static ABI_ret ret () noexcept
	{
		ABI_ret abi;
		new (&abi) Var ();
		return abi;
	}

};

}
}

#endif
