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
#ifndef NIRVANA_ORB_TYPECODE_S_H_
#define NIRVANA_ORB_TYPECODE_S_H_

#include "TypeCode.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, TypeCode>
{
public:
	static const typename Bridge <TypeCode>::EPV epv_;

protected:
	static ABI_boolean _equal (Bridge <TypeCode>* _b, Interface* other, Interface* _env)
	{
		try {
			return S::_implementation (_b).equal (TypeI <TypeCode>::in (other));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ABI_boolean _equivalent (Bridge <TypeCode>* _b, Interface* other, Interface* _env)
	{
		try {
			return S::_implementation (_b).equivalent (TypeI <TypeCode>::in (other));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _get_compact_typecode (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).get_compact_typecode ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ABI_enum _kind (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <TCKind>::ret (S::_implementation (_b).kind ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Type <String>::ABI_ret _id (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <String>::ret (S::_implementation (_b).id ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <String>::ret ();
	}

	static Type <String>::ABI_ret _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <String>::ret (S::_implementation (_b).name ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <String>::ret ();
	}

	static ULong _member_count (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).member_count ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Type <String>::ABI_ret _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		try {
			return Type <String>::ret (S::_implementation (_b).member_name (index));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <String>::ret ();
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).member_type (index));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Type <Any>::ABI_ret _member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		try {
			return Type <Any>::ret (S::_implementation (_b).member_label (index));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Any>::ret ();
	}

	static Interface* _discriminator_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).discriminator_type ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Long _default_index (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).default_index ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ULong _length (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).length ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _content_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).content_type ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static UShort _fixed_digits (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).fixed_digits ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Short _fixed_scale (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).fixed_scale ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Visibility _member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		try {
			return S::_implementation (_b).member_visibility (index);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).type_modifier ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return TypeI <TypeCode>::ret (S::_implementation (_b).concrete_base_type ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::Size __size (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b)._size ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void __construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		try {
			S::_implementation (_b)._construct (p);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		try {
			S::_implementation (_b)._destruct (p);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, Interface* _env)
	{
		try {
			S::_implementation (_b)._copy (dst, src);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, Interface* _env)
	{
		try {
			S::_implementation (_b)._move (dst, src);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static ABI_boolean __has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b)._has_marshal ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void __marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b)._marshal_in (src, TypeI <Marshal>::in (marshaler), dst);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b)._marshal_out (src, TypeI <Marshal>::in (marshaler), dst);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __unmarshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* unmarshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b)._unmarshal (src, TypeI <Unmarshal>::in (unmarshaler), dst);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge <TypeCode>::EPV Skeleton <S, TypeCode>::epv_ = {
	{ // header
		Bridge <TypeCode>::repository_id_,
		S::template __duplicate <TypeCode>,
		S::template __release <TypeCode>
	},
	{ // epv
		S::_equal,
		S::_equivalent,
		S::_get_compact_typecode,
		S::_kind,
		S::_id,
		S::_name,
		S::_member_count,
		S::_member_name,
		S::_member_type,
		S::_member_label,
		S::_discriminator_type,
		S::_default_index,
		S::_length,
		S::_content_type,
		S::_fixed_digits,
		S::_fixed_scale,
		S::_member_visibility,
		S::_type_modifier,
		S::_concrete_base_type,

		S::__size,
		S::__construct,
		S::__destruct,
		S::__copy,
		S::__move,
		S::__has_marshal,
		S::__marshal_in,
		S::__marshal_out,
		S::__unmarshal
	}
};

template <class S>
class ServantStatic <S, TypeCode> :
	public ImplementationPseudoStatic <S, TypeCode>
{};

}
}

#endif
