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

#include "../TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Internal {

template <class U>
class TypeCodeUnion :
	public TypeCodeStatic <TypeCodeUnion <U>,
		TypeCodeWithId <TCKind::tk_union, U>, TypeCodeOps <U> >,
	public TypeCodeMembers <U>,
	public TypeCodeName <U>,
	public TypeCodeORB
{
	typedef TypeCodeStatic <TypeCodeUnion <U>,
		TypeCodeWithId <TCKind::tk_union, U>, TypeCodeOps <U> > Base;
	typedef TypeCodeMembers <U> Members;

	typedef typename Type <U>::DiscriminatorType DiscriminatorType;
public:
	using TypeCodeName <U>::_s_name;
	using Members::_s_member_count;
	using Members::_s_member_name;
	using Members::_s_member_type;
	using TypeCodeORB::_s_equal;
	using TypeCodeORB::_s_equivalent;
	using TypeCodeORB::_s_get_compact_typecode;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return orb->tc_equal (Base::_get_ptr (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return orb->tc_equivalent (Base::_get_ptr (), other);
	}

	static Type <CORBA::TypeCode>::VRet get_compact_typecode ()
	{
		return orb->get_compact_typecode (Base::_get_ptr ());
	}

	static Type <Any>::ABI_ret _s_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= countof (labels_)) {
			set_Bounds (_env);
			return Type <Any>::ret ();
		}

		Any ret;
		if (index != Type <U>::default_index_)
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
		return Type <U>::default_index_;
	}

private:
	static I_ptr <TypeCode> discriminator_tc_ptr () noexcept
	{
		return Type <typename Type <U>::DiscriminatorType>::type_code ();
	}

	static bool labels_equal (I_ptr <TypeCode> other);

private:
	static const DiscriminatorType labels_ [];
};

template <class U>
bool TypeCodeUnion <U>::labels_equal (I_ptr <TypeCode> other)
{
	for (ULong i = 0; i < countof (labels_); ++i) {
		Any label_any = other->member_label (i);
		DiscriminatorType label;
		if (!(label_any >>= label) || labels_ [i] != label)
			return false;
	}
	return true;
}

}
}

#endif
