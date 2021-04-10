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
#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeByVal.h"
#include "TypeByRef.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
/// 
/// \tparam T The variable type.
/// \tparam ABI The ABI type
template <typename T, typename TABI = T, typename TMember = T>
struct TypeFixLen : std::conditional_t <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T, TABI, TMember>, TypeByRef <T, TABI, TMember> >
{
	typedef std::conditional_t <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T, TABI, TMember>, TypeByRef <T, TABI, TMember> > Base;

	typedef typename Base::Var_type Var_type;
	typedef typename Base::ABI_type ABI_type;

	static const bool has_check = false;
	static void check (const ABI_type&) {}

	static const bool has_marshal = false;

	static void marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (typename Type <T>::ABI_type&)src;
	}

	static void marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = (ABI_type&)src;
	}

	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst) NIRVANA_NOEXCEPT
	{
		dst = Type <T>::in (src);
	}
};

template <typename T
#ifdef NIRVANA_C11
	, typename Enable = void
#endif
>
using TypeTrivial = TypeFixLen <T>;

template <typename T>
struct Type : TypeTrivial <T
#ifdef NIRVANA_C11
	, typename std::enable_if_t <std::is_trivially_copyable <T>::value>
#endif
>
{};

}
}

#endif
