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

#include "../Server.h"
#include "../TypeCode_s.h"
#include "InterfaceMetadata.h"
#include <new>

namespace CORBA {
namespace Nirvana {

void set_BadKind (Interface* env);
void set_Bounds (Interface* env);

template <size_t cc> inline
Type <String>::ABI_ret const_string_ret (Char const (&s) [cc])
{
	StringBase <Char> sb (s);
	return Type <String>::ret (std::move (static_cast <String&> (sb)));
}

inline
Type <String>::ABI_ret const_string_ret_p (const Char* s)
{
	StringBase <Char> sb (s);
	return Type <String>::ret (std::move (static_cast <String&> (sb)));
}

class TypeCodeBase
{
public:
	static Boolean equal (TCKind tk, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, TypeCode_ptr other);

	static Type <String>::ABI_ret _id (Bridge <TypeCode>* _b, Interface* _env);
	static Type <String>::ABI_ret _name (Bridge <TypeCode>* _b, Interface* _env);
	static ULong _member_count (Bridge <TypeCode>* _b, Interface* _env);
	static Type <String>::ABI_ret _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env);
	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env);
	static Type <Any>::ABI_ret _member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env);
	static Interface* _discriminator_type (Bridge <TypeCode>* _b, Interface* _env);
	static Long _default_index (Bridge <TypeCode>* _b, Interface* _env);
	static ULong _length (Bridge <TypeCode>* _b, Interface* _env);
	static Interface* _content_type (Bridge <TypeCode>* _b, Interface* _env);
	static UShort _fixed_digits (Bridge <TypeCode>* _b, Interface* _env);
	static Short _fixed_scale (Bridge <TypeCode>* _b, Interface* _env);
	static Visibility _member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env);
	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, Interface* _env);
	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, Interface* _env);

protected:
	static Boolean equal (TCKind tk, String_in& id, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, String_in& id, TypeCode_ptr other);
	static TypeCode_var dereference_alias (TypeCode_ptr tc);
	static Boolean equal (const Char* const* members, ULong member_cnt, TypeCode_ptr other);
	static Boolean equivalent (const Char* const* members, ULong member_cnt, TypeCode_ptr other);
	static Boolean equal (const Parameter* members, ULong member_cnt, TypeCode_ptr other);
	static Boolean equivalent (const Parameter* members, ULong member_cnt, TypeCode_ptr other);
};

template <TCKind tk>
class TypeCodeTK :
	public TypeCodeBase
{
public:
	static const TCKind tk_ = tk;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk_, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk_, TypeCodeBase::dereference_alias (other));
	}

	static Type <TCKind>::ABI_ret _kind (Bridge <TypeCode>* _b, Interface* _env)
	{
		return tk_;
	}
};

template <TCKind tk, class T>
class TypeCodeWithId :
	public TypeCodeTK <tk>
{
public:
	typedef T RepositoryType;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk, RepositoryType::repository_id_, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk, RepositoryType::repository_id_, TypeCodeBase::dereference_alias (other));
	}

	static ABI <String> _id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (RepositoryType::repository_id_);
	}
};

template <class T>
class TypeCodeName
{
public:
	static Type <String>::ABI_ret _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (name_);
	}

	static Boolean equal (TypeCode_ptr other)
	{
		return other->name ().compare (0, countof (name_) - 1, name_) == 0;
	}

private:
	static const Char name_ [];
};

template <typename T>
class TypeCodeOps
{
public:
	typedef typename Type <T>::Var_type Var_type;
	typedef typename Type <T>::ABI_type ABI_type;

	static size_t __size (Bridge <TypeCode>* _b, Interface* _env)
	{
		return sizeof (Var_type);
	}

	static void _construct (::Nirvana::Pointer p)
	{
		_check_pointer (p);
		new (p) Var_type ();
	}

	static void _destruct (::Nirvana::Pointer p)
	{
		if (p)
			reinterpret_cast <Var_type*> (p)->~Var_type ();
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI_type*> (src));
		new (dst) Var_type (*reinterpret_cast <const Var_type*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI_type*> (src));
		new (dst) Var_type (std::move (*reinterpret_cast <Var_type*> (src)));
	}

	static Type <Boolean>::ABI_ret __has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		return Type <T>::has_marshal;
	}

	static void _marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI_type*> (src));
		Type <T>::marshal_in (*reinterpret_cast <const Var_type*> (src), marshaler, *reinterpret_cast <ABI_type*> (dst));
	}

	static void _marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI_type*> (src));
		Type <T>::marshal_out (*reinterpret_cast <Var_type*> (src), marshaler, *reinterpret_cast <ABI_type*> (dst));
	}

	static void _unmarshal (::Nirvana::ConstPointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		// Do not call check() here, unmarshal() will check.
		Type <T>::unmarshal (*reinterpret_cast <const ABI_type*> (src), unmarshaler, *reinterpret_cast <Var_type*> (dst));
	}
};

template <>
class TypeCodeOps <Boolean>
{
public:
	typedef typename Type <Boolean>::Member_type Var_type;

	static size_t __size (Bridge <TypeCode>* _b, Interface* _env)
	{
		return sizeof (Var_type);
	}

	static void _construct (::Nirvana::Pointer p)
	{
		_check_pointer (p);
	}

	static void _destruct (::Nirvana::Pointer p)
	{
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		*reinterpret_cast <Var_type*> (dst) = *reinterpret_cast <const Var_type*> (src);
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		*reinterpret_cast <Var_type*> (dst) = *reinterpret_cast <const Var_type*> (src);
	}

	static Type <Boolean>::ABI_ret __has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		return 0;
	}

	static void _marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		*reinterpret_cast <Var_type*> (dst) = *reinterpret_cast <const Var_type*> (src);
	}

	static void _marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		*reinterpret_cast <Var_type*> (dst) = *reinterpret_cast <const Var_type*> (src);
	}

	static void _unmarshal (::Nirvana::ConstPointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		*reinterpret_cast <Var_type*> (dst) = *reinterpret_cast <const Var_type*> (src);
	}
};

template <>
class TypeCodeOps <void>
{
public:
	typedef void Valtype;

	static size_t __size (Bridge <TypeCode>* _b, Interface* _env)
	{
		return 0;
	}

	static void __construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{}

	static void __destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{}

	static void __copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, Interface* _env)
	{}

	static void __move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, Interface* _env)
	{}

	static Type <Boolean>::ABI_ret __has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		return 0;
	}

	static void __marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{}

	static void __marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{}

	static void __unmarshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* unmarshaler, ::Nirvana::Pointer, Interface* _env)
	{}
};

template <class S, class Impl, class Ops>
class TypeCodeImplBase :
	public S, public Impl, public Ops
{
public:
	using Impl::_kind;
	using Impl::_id;
	using Impl::_name;
	using Impl::_member_count;
	using Impl::_member_name;
	using Impl::_member_type;
	using Impl::_member_label;
	using Impl::_discriminator_type;
	using Impl::_default_index;
	using Impl::_length;
	using Impl::_content_type;
	using Impl::_fixed_digits;
	using Impl::_fixed_scale;
	using Impl::_member_visibility;
	using Impl::_type_modifier;
	using Impl::_concrete_base_type;

	using Ops::__size;
	using Ops::__has_marshal;

	// The get_compact_typecode operation strips out all optional name and member
	// name fields, but it leaves all alias typecodes intact.
	TypeCode_var get_compact_typecode ()
	{
		// Currently, we don't strip names, just return this type code.
		return TypeCode::_duplicate (&static_cast <TypeCode&> (static_cast <Bridge <TypeCode>&> (static_cast <S&> (*this))));
	}
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
	using TypeCodeOps <void>::__construct;
	using TypeCodeOps <void>::__destruct;
	using TypeCodeOps <void>::__copy;
	using TypeCodeOps <void>::__move;
	using TypeCodeOps <void>::__marshal_in;
	using TypeCodeOps <void>::__marshal_out;
	using TypeCodeOps <void>::__unmarshal;
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
	static ULong _length (Bridge <TypeCode>* _b, Interface* _env)
	{
		return bound;
	}
};

// for tk_sequence, tk_array, tk_value_box, tk_alias
template <class Content>
class TypeCodeContentType
{
public:
	static Interface* _content_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		Interface* tc = &ptr ();
		return (tc->_epv ().duplicate) (tc, _env);
	}

protected:
	static TypeCode_ptr ptr ()
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

	static ULong _member_count (Bridge <TypeCode>* _b, Interface* _env)
	{
		return member_count_;
	}
};

}
}

#endif
