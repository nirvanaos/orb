/// \file TypeCodeImpl.h
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
#ifndef NIRVANA_ORB_TYPECODEIMPL_H_
#define NIRVANA_ORB_TYPECODEIMPL_H_
#pragma once

#include "ImplementationPseudo.h"
#include "ImplementationPseudoStatic.h"
#include "TypeCode_s.h"
#include "TypeCodeOps.h"
#include "StaticIdNameTC.h"

namespace CORBA {
namespace Internal {

/// Function to return TypeCode
typedef I_ptr <TypeCode> (*GetTypeCode) ();

void set_BadKind (Interface* env) noexcept;
void set_Bounds (Interface* env) noexcept;

template <size_t cc> inline
Type <String>::ABI_ret const_string_ret (Char const (&s) [cc]) noexcept
{
	StringView <Char> sb (s);
	return Type <String>::ret (std::move (reinterpret_cast <String&> (sb)));
}

inline
Type <String>::ABI_ret const_string_ret_p (const Char* s) noexcept
{
	StringView <Char> sb (s);
	return Type <String>::ret (std::move (reinterpret_cast <String&> (sb)));
}

struct Parameter;

class TypeCodeBase
{
public:
	static Type <String>::ABI_ret _s_id (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Type <String>::ABI_ret _s_name (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Interface* _s_get_compact_typecode (Bridge <CORBA::TypeCode>* _b, Interface* _env) noexcept;
	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept;
	static Interface* _s_member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept;
	static Type <Any>::ABI_ret _s_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept;
	static Interface* _s_discriminator_type (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Long _s_default_index (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static ULong _s_length (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Interface* _s_content_type (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static UShort _s_fixed_digits (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Short _s_fixed_scale (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Visibility _s_member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept;
	static ValueModifier _s_type_modifier (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Interface* _s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env) noexcept;

	static I_ptr <TypeCode> dereference_alias (I_ptr <TypeCode> tc);

protected:
	enum class EqResult
	{
		YES,
		NO,
		UNKNOWN
	};

	static EqResult equivalent_ (TCKind tk, String_in id, I_ptr <TypeCode> other);

	// Scalar

	static Boolean equal (TCKind tk, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, I_ptr <TypeCode> other);

	// String

	static Boolean equal (TCKind tk, ULong bound, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> other);

	// Value box and alias

	static Boolean equal (TCKind tk, String_in id, String_in name, I_ptr <TypeCode> content, I_ptr <TypeCode> other);

	// Value box

	static Boolean equivalent (TCKind tk, String_in id, I_ptr <TypeCode> content, I_ptr <TypeCode> other);

	// Sequence and array

	static Boolean equal (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other);

	// ID & name: 

	static Boolean equal (TCKind tk, String_in id, String_in name, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, String_in id, I_ptr <TypeCode> other);
	static Boolean equal (TCKind tk, String_in id, I_ptr <TypeCode> other);

	// Enum

	static Boolean equal (TCKind tk, String_in id, String_in name,
		const Char* const* members, ULong member_cnt, I_ptr <TypeCode> other);
	static EqResult equivalent_ (TCKind tk, String_in id, ULong member_cnt,
		I_ptr <TypeCode> other);

};

// TypeCode implementation with delegating some operations to ORB.
class TypeCodeORB
{
public:
	static Type <CORBA::Boolean>::ABI_ret _s_equal (Bridge <CORBA::TypeCode>* _b, Interface* tc, Interface* _env) noexcept;
	static Type <CORBA::Boolean>::ABI_ret _s_equivalent (Bridge <CORBA::TypeCode>* _b, Interface* tc, Interface* _env) noexcept;
	static Interface* _s_get_compact_typecode (Bridge <CORBA::TypeCode>* _b, Interface* _env) noexcept;
};

template <TCKind tk>
class TypeCodeTK :
	public TypeCodeBase
{
public:
	static const TCKind tk_ = tk;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (tk_, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equivalent (tk_, other);
	}

	static Type <TCKind>::ABI_ret _s_kind (Bridge <TypeCode>* _b, Interface* _env)
	{
		return (ABI_enum)tk_;
	}
};

template <TCKind tk, class T>
class TypeCodeWithId :
	public TypeCodeTK <tk>
{
public:
	typedef RepIdOf <T> RepositoryType;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (tk, RepositoryType::id, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equivalent (tk, RepositoryType::id, other);
	}

	static Type <String>::ABI_ret _s_id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (RepositoryType::id);
	}
};

// Use inline anonimous namespace to avoid linker errors "duplicated symbol".
inline namespace {

template <class T>
class TypeCodeName
{
public:
	static Type <String>::ABI_ret _s_name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (name_);
	}

protected:
	static const Char name_ [];
};

}

template <class S, class Impl, class Ops>
class TypeCodeImplBase :
	public S, public Impl, public Ops
{
public:
	using Impl::_s_kind;
	using Impl::_s_id;
	using Impl::_s_name;
	using Impl::_s_get_compact_typecode;
	using Impl::_s_member_count;
	using Impl::_s_member_name;
	using Impl::_s_member_type;
	using Impl::_s_member_label;
	using Impl::_s_discriminator_type;
	using Impl::_s_default_index;
	using Impl::_s_length;
	using Impl::_s_content_type;
	using Impl::_s_fixed_digits;
	using Impl::_s_fixed_scale;
	using Impl::_s_member_visibility;
	using Impl::_s_type_modifier;
	using Impl::_s_concrete_base_type;

	using Ops::_s_n_aligned_size;
	using Ops::_s_n_CDR_size;
	using Ops::_s_n_align;
	using Ops::_s_n_is_CDR;

};

template <class S, class Impl, class Ops>
class TypeCodeImpl :
	public TypeCodeImplBase <S, Impl, Ops>
{};

template <class S, class Impl>
class TypeCodeImpl <S, Impl, TypeCodeOps <void> > :
	public TypeCodeImplBase <S, Impl, TypeCodeOps <void> >
{
public:
	using TypeCodeOps <void>::_s_n_construct;
	using TypeCodeOps <void>::_s_n_destruct;
	using TypeCodeOps <void>::_s_n_copy;
	using TypeCodeOps <void>::_s_n_move;
	using TypeCodeOps <void>::_s_n_marshal_in;
	using TypeCodeOps <void>::_s_n_marshal_out;
	using TypeCodeOps <void>::_s_n_unmarshal;
	using TypeCodeOps <void>::_s_n_byteswap;
};

template <class S, class Impl, class Ops>
class TypeCodeStatic :
	public TypeCodeImpl <ServantStatic <S, TypeCode>, Impl, Ops>
{};

// for tk_string, tk_sequence, and tk_array
template <ULong bound = 0>
class TypeCodeLength
{
public:
	static ULong _s_length (Bridge <TypeCode>* _b, Interface* _env)
	{
		return bound;
	}
};

// for tk_sequence, tk_array, tk_value_box, tk_alias
template <class Content>
class TypeCodeContentType
{
public:
	static Interface* _s_content_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		Interface* tc = &ptr ();
		return (tc->_epv ().duplicate) (tc, _env);
	}

protected:
	static I_ptr <TypeCode> ptr () noexcept
	{
		return Type <Content>::type_code ();
	}
};

// for tk_struct, tk_union, tk_enum, tk_value, tk_except
template <ULong member_count>
class TypeCodeMemberCount
{
public:
	static const ULong member_count_ = member_count;

	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env)
	{
		return member_count_;
	}
};

class TypeCodeStaticIdName
{
public:
	static Type <String>::ABI_ret _s_id (Bridge <TypeCode>* _b, Interface* _env);
	static Type <String>::ABI_ret _s_name (Bridge <TypeCode>* _b, Interface* _env);
};

class TypeCodeValueAbstractBase
{
public:
	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept;
	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept;
	static Interface* _s_member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept;
	static Visibility _s_member_visibility (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept;
	static ValueModifier _s_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
		noexcept;
	static Interface* _s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
		noexcept;
};

}
}

#endif
