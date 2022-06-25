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
#ifndef NIRVANA_ORB_TYPECODEUNION_H_
#define NIRVANA_ORB_TYPECODEUNION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Internal {

template <class S>
class TypeCodeUnion :
	public TypeCodeStatic <TypeCodeUnion <S>,
		TypeCodeWithId <TCKind::tk_union, S>, TypeCodeOps <S> >,
	public TypeCodeMembers <S>,
	public TypeCodeName <S>
{
	typedef TypeCodeStatic <TypeCodeUnion <S>,
		TypeCodeWithId <TCKind::tk_union, S>, TypeCodeOps <S> > Base;
	typedef TypeCodeMembers <S> Members;

	typedef typename Type <S>::DiscriminatorType DiscriminatorType;
public:
	using TypeCodeName <S>::_s_name;
	using Members::_s_member_count;
	using Members::_s_member_name;
	using Members::_s_member_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (TCKind::tk_union, Base::RepositoryType::id,
			TypeCodeName <S>::name_,
			Members::members (), Members::member_count (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equivalent (TCKind::tk_union, Base::RepositoryType::id,
			Members::members (), Members::member_count (), other);
	}

	static Type <Any>::ABI_ret _s_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= countof (labels_)) {
			set_Bounds (_env);
			return Type <Any>::ret ();
		}

		Any ret;
		if (index != Type <S>::default_index_)
			ret <<= labels_ [index];
		else
			ret <<= Any::from_octet (0);
		return Type <Any>::ret (std::move (ret));
	}

	static Interface* _s_discriminator_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		Interface* tc = &discriminator_tc_ptr ();
		return (tc->_epv ().duplicate) (tc, _env);
	}

	static Long _s_default_index (Bridge <TypeCode>* _b, Interface* _env)
	{
		return Type <S>::default_index_;
	}

private:
	static I_ptr <TypeCode> discriminator_tc_ptr () NIRVANA_NOEXCEPT
	{
		return Type <typename Type <S>::DiscriminatorType>::type_code ();
	}

private:
	static const DiscriminatorType labels_ [];
};

}
}

#endif
