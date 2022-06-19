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
namespace Internal {

void set_BadKind (Interface* env) NIRVANA_NOEXCEPT;
void set_Bounds (Interface* env) NIRVANA_NOEXCEPT;

template <size_t cc> inline
Type <String>::ABI_ret const_string_ret (Char const (&s) [cc]) NIRVANA_NOEXCEPT
{
	StringBase <Char> sb (s);
	return Type <String>::ret (std::move (static_cast <String&> (sb)));
}

inline
Type <String>::ABI_ret const_string_ret_p (const Char* s) NIRVANA_NOEXCEPT
{
	StringBase <Char> sb (s);
	return Type <String>::ret (std::move (static_cast <String&> (sb)));
}

struct StateMember
{
	const Char* name;
	const GetTypeCode type;
	Visibility visibility;
};

class TypeCodeBase
{
public:
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
	static I_ptr <TypeCode> dereference_alias (I_ptr <TypeCode> tc);

	// Scalar
	static Boolean equal (TCKind tk, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, I_ptr <TypeCode> other);

	// String
	static Boolean equal (TCKind tk, ULong bound, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> other);

	// Sequence and array
	static Boolean equal (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other);

	static Boolean equal (TCKind tk, String_in& id, I_ptr <TypeCode> other);
	static Boolean equivalent (TCKind tk, String_in& id, I_ptr <TypeCode> other);

	static Boolean equal (TCKind tk, String_in& id, String_in& name, I_ptr <TypeCode> other);

	static Boolean equal (TCKind tk, String_in& id, String_in& name,
		const Char* const* members, ULong member_cnt, I_ptr <TypeCode> other);

	static Boolean equivalent (TCKind tk, String_in& id, ULong member_cnt,
		I_ptr <TypeCode> other);

	static Boolean equal (TCKind tk, String_in& id, String_in& name,
		const Parameter* members, ULong member_cnt, I_ptr <TypeCode> other);

	static Boolean equivalent (TCKind tk, String_in& id,
		const Parameter* members, ULong member_cnt, I_ptr <TypeCode> other);

	// Value type

	static Boolean equal (Bridge <TypeCode>* bridge, String_in& id, String_in& name,
		ValueModifier mod, GetTypeCode base,
		const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other);

	static Boolean equivalent (Bridge <TypeCode>* bridge, String_in& id,
		ValueModifier mod, GetTypeCode base,
		const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other);

private:
	enum class EqResult
	{
		YES,
		NO,
		UNKNOWN
	};

	static EqResult equivalent_ (TCKind tk, String_in& id, I_ptr <TypeCode> other);

	// To prevent recursion for value types, use simply set implemented over std::vector.

	struct CompareEntry
	{
		Bridge <TypeCode>* first, * second;

		bool operator == (const CompareEntry& rhs) const
		{
			return first == rhs.first && second == rhs.second;
		}
	};

	class CompareSet :
		private std::vector <CompareEntry>
	{
	public:
		CompareSet (Bridge <TypeCode>* first, Bridge <TypeCode>* second) :
			std::vector <CompareEntry> (1, { first, second })
		{}

		bool insert (Bridge <TypeCode>* first, Bridge <TypeCode>* second)
		{
			CompareEntry ce{ first, second };
			if (std::find (begin (), end (), ce) == end ()) {
				push_back (ce);
				return true;
			}
			return false;
		}
	};

	// Compare value members

	static Boolean equal (CompareSet& cs,
		I_ptr <TypeCode> left, I_ptr <TypeCode> right);
	static Boolean equivalent (CompareSet& cs,
		I_ptr <TypeCode> left, I_ptr <TypeCode> right);
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

	static Type <TCKind>::ABI_ret _kind (Bridge <TypeCode>* _b, Interface* _env)
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

	static Boolean equal (I_ptr <TypeCode> other) NIRVANA_NOEXCEPT
	{
		return TypeCodeBase::equal (tk, RepositoryType::id_, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other) NIRVANA_NOEXCEPT
	{
		return TypeCodeBase::equivalent (tk, RepositoryType::id_, other);
	}

	static ABI <String> _id (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (RepositoryType::id_);
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

protected:
	static const Char name_ [];
};

template <typename T>
class TypeCodeOps
{
public:
	typedef typename Type <T>::ABI ABI;
	typedef typename std::conditional <std::is_same <T, Boolean>::value,
		Type <Boolean>::ABI, typename Type <T>::Var>::type Var;

	static size_t _n_size (Bridge <TypeCode>*, Interface*)
	{
		return sizeof (Var);
	}

	static Type <Boolean>::ABI_ret _n_fixed_len (Bridge <TypeCode>*, Interface*)
	{
		return Type <T>::fixed_len;
	}

	static void n_construct (void* p)
	{
		check_pointer (p);
		new (p) Var ();
	}

	static void n_destruct (void* p)
	{
		check_pointer (p);
		if (p)
			reinterpret_cast <Var*> (p)->~Var ();
	}

	static void n_copy (void* dst, const void* src)
	{
		check_pointer (dst);
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		new (dst) Var (*reinterpret_cast <const Var*> (src));
	}

	static void n_move (void* dst, void* src)
	{
		check_pointer (dst);
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		new (dst) Var (std::move (*reinterpret_cast <Var*> (src)));
	}

	static void n_marshal_in (const void* src, size_t count, IORequest_ptr rq)
	{
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		Type <T>::marshal_in_a (reinterpret_cast <const Var*> (src), count, rq);
	}

	static void n_marshal_out (void* src, size_t count, IORequest_ptr rq)
	{
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		Type <T>::marshal_out_a (reinterpret_cast <Var*> (src), count, rq);
	}

	static void n_unmarshal (IORequest_ptr rq, size_t count, void* dst)
	{
		check_pointer (dst);
		// Do not call check() here, unmarshal() will check.
		Type <T>::unmarshal_a (rq, count, reinterpret_cast <Var*> (dst));
	}
};

template <>
class TypeCodeOps <void>
{
public:
	typedef void Valtype;

	static size_t _n_size (Bridge <TypeCode>*, Interface*)
	{
		return 0;
	}

	static Type <Boolean>::ABI_ret _n_fixed_len (Bridge <TypeCode>*, Interface*)
	{
		return true;
	}

	static void _n_construct (Bridge <TypeCode>*, void*, Interface*)
	{}

	static void _n_destruct (Bridge <TypeCode>*, void*, Interface*)
	{}

	static void _n_copy (Bridge <TypeCode>*, void*, const void*, Interface*)
	{}

	static void _n_move (Bridge <TypeCode>*, void*, void*, Interface*)
	{}

	static void _n_marshal_in (Bridge <TypeCode>*, const void*, size_t,
		Interface*, Interface*)
	{}

	static void _n_marshal_out (Bridge <TypeCode>*, void*, size_t,
		Interface*, Interface*)
	{}

	static void _n_unmarshal (Bridge <TypeCode>*, Interface*, size_t,
		void*, Interface*)
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

	using Ops::_n_size;
	using Ops::_n_fixed_len;

	// The get_compact_typecode operation strips out all optional name and member
	// name fields, but it leaves all alias typecodes intact.
	I_ref <TypeCode> get_compact_typecode () NIRVANA_NOEXCEPT
	{
		// Currently, we don't strip names, just return this type code.
		return I_ptr <TypeCode> (&static_cast <TypeCode&> (static_cast <Bridge <TypeCode>&> (static_cast <S&> (*this))));
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
	using TypeCodeOps <void>::_n_construct;
	using TypeCodeOps <void>::_n_destruct;
	using TypeCodeOps <void>::_n_copy;
	using TypeCodeOps <void>::_n_move;
	using TypeCodeOps <void>::_n_marshal_in;
	using TypeCodeOps <void>::_n_marshal_out;
	using TypeCodeOps <void>::_n_unmarshal;
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
	static I_ptr <TypeCode> ptr () NIRVANA_NOEXCEPT
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
