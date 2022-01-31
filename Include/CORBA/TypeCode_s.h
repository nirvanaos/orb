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
#pragma once

#include "TypeCode.h"

namespace CORBA {
namespace Internal {

template <class S>
class Skeleton <S, TypeCode>
{
public:
	static const typename Bridge <TypeCode>::EPV epv_;

protected:
	static Type <Boolean>::ABI_ret _equal (Bridge <TypeCode>* _b, Interface* other, Interface* _env)
	{
		try {
			return S::_implementation (_b).equal (Type <TypeCode>::in (other));
		} catch (Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Type <Boolean>::ABI_ret _equivalent (Bridge <TypeCode>* _b, Interface* other, Interface* _env)
	{
		try {
			return S::_implementation (_b).equivalent (Type <TypeCode>::in (other));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _get_compact_typecode (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <TypeCode>::ret (S::_implementation (_b).get_compact_typecode ());
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <String>::ret ();
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		try {
			return Type <TypeCode>::ret (S::_implementation (_b).member_type (index));
		} catch (Exception& e) {
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
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <Any>::ret ();
	}

	static Interface* _discriminator_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <TypeCode>::ret (S::_implementation (_b).discriminator_type ());
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _content_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <TypeCode>::ret (S::_implementation (_b).content_type ());
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
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
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <TypeCode>::ret (S::_implementation (_b).concrete_base_type ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static ::Nirvana::Size _n_size (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return S::_implementation (_b).n_size ();
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _n_construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		try {
			S::_implementation (_b).n_construct (p);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _n_destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, Interface* _env)
	{
		try {
			S::_implementation (_b).n_destruct (p);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _n_copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, Interface* _env)
	{
		try {
			S::_implementation (_b).n_copy (dst, src);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _n_move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, Interface* _env)
	{
		try {
			S::_implementation (_b).n_move (dst, src);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static Type <Boolean>::ABI_ret _n_has_marshal (Bridge <TypeCode>* _b, Interface* _env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (_b).n_has_marshal ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _n_marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b).n_marshal_in (src, Type <Marshal>::in (marshaler), dst);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _n_marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b).n_marshal_out (src, Type <Marshal>::in (marshaler), dst);
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _n_unmarshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* unmarshaler, ::Nirvana::Pointer dst, Interface* _env)
	{
		try {
			S::_implementation (_b).n_unmarshal (src, Type <Unmarshal>::in (unmarshaler), dst);
		} catch (Exception& e) {
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

		S::_n_size,
		S::_n_construct,
		S::_n_destruct,
		S::_n_copy,
		S::_n_move,
		S::_n_has_marshal,
		S::_n_marshal_in,
		S::_n_marshal_out,
		S::_n_unmarshal
	}
};

template <class S>
class ServantStatic <S, TypeCode> :
	public ImplementationPseudoStatic <S, TypeCode>
{};

}
}

#endif
