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
#include "../../pch/pch.h"

namespace CORBA {
namespace Internal {

void set_BadKind (Interface* env) noexcept
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::BadKind>::id, nullptr);
}

void set_Bounds (Interface* env) noexcept
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::Bounds>::id, nullptr);
}

I_ptr <TypeCode> TypeCodeBase::dereference_alias (I_ptr <TypeCode> tc)
{
	while (tc && TCKind::tk_alias == tc->kind ()) {
		// tc->content_type () returns _ref_type
		// but we can safely release it because it is always held.
		tc = tc->content_type ();
	}
	return tc;
}

Boolean TypeCodeBase::equal (TCKind tk, I_ptr <TypeCode> other)
{
	return other && tk == other->kind ();
}

Boolean TypeCodeBase::equivalent (TCKind tk, I_ptr <TypeCode> other)
{
	return equal (tk, dereference_alias (other));
}

Boolean TypeCodeBase::equal (TCKind tk, ULong bound, I_ptr <TypeCode> other)
{
	return tk == other->kind () && other->length () == bound;
}

Boolean TypeCodeBase::equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> other)
{
	return equal (tk, bound, dereference_alias (other));
}

Boolean TypeCodeBase::equal (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	return equal (tk, bound, other) && content->equal (other->content_type ());
}

Boolean TypeCodeBase::equivalent (TCKind tk, ULong bound, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	I_ptr <TypeCode> tco = dereference_alias (other);
	return equal (tk, bound, tco) && content->equivalent (tco->content_type ());
}

Boolean TypeCodeBase::equal (TCKind tk, String_in id, String_in name, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	return equal (tk, id, name, other) && content->equal (other->content_type ());
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in id, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	I_ptr <TypeCode> tco = dereference_alias (other);
	EqResult eq = equivalent_ (tk, id, tco);
	if (EqResult::UNKNOWN != eq)
		return EqResult::NO != eq;
	return content->equal (tco->content_type ());
}

Boolean TypeCodeBase::equal (TCKind tk, String_in id, I_ptr <TypeCode> other)
{
	return equal (tk, other)
		&& static_cast <const String&> (id) == other->id ();
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in id, I_ptr <TypeCode> other)
{
	return equal (tk, id, dereference_alias (other));
}

Boolean TypeCodeBase::equal (TCKind tk, String_in id, String_in name, I_ptr <TypeCode> other)
{
	if (!equal (tk, id, other))
		return false;

	return static_cast <const String&> (name) == other->name ();
}

Boolean TypeCodeBase::equal (TCKind tk, String_in id, String_in name,
	const Char* const* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	if (!equal (tk, id, name, other))
		return false;

	if (other->member_count () != member_cnt)
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (other->member_name (i) != members [i])
			return false;
	}
	return true;
}

TypeCodeBase::EqResult TypeCodeBase::equivalent_ (TCKind tk, String_in id, I_ptr <TypeCode> other)
{
	assert (other->kind () != TCKind::tk_alias);
	if (!equal (tk, other))
		return EqResult::NO;
	const String oid = other->id ();
	if (!id.empty () && !oid.empty ())
		return static_cast <const String&> (id) == oid ? EqResult::YES : EqResult::NO;
	return EqResult::UNKNOWN;
}

TypeCodeBase::EqResult TypeCodeBase::equivalent_ (TCKind tk, String_in id, ULong member_cnt, I_ptr <TypeCode> other)
{
	EqResult eq = equivalent_ (tk, id, other);
	if (EqResult::UNKNOWN == eq && member_cnt != other->member_count ())
		eq = EqResult::NO;
	return eq;
}

Type <String>::ABI_ret TypeCodeBase::_s_id (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Type <String>::ABI_ret TypeCodeBase::_s_name (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Interface* TypeCodeBase::_s_get_compact_typecode (Bridge <CORBA::TypeCode>* _b, Interface* _env) noexcept
{
	// The get_compact_typecode operation strips out all optional name and member
	// name fields, but it leaves all alias typecodes intact.
	// By default, we don't strip names, just return this type code.
	return _b->_epv ().header.duplicate (_b, _env);
}

ULong TypeCodeBase::_s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

Type <String>::ABI_ret TypeCodeBase::_s_member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Interface* TypeCodeBase::_s_member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept
{
	set_BadKind (_env);
	return nullptr;
}

Type <Any>::ABI_ret TypeCodeBase::_s_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept
{
	set_BadKind (_env);
	return Type <Any>::ret ();
}

Interface* TypeCodeBase::_s_discriminator_type (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return nullptr;
}

Long TypeCodeBase::_s_default_index (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

ULong TypeCodeBase::_s_length (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_s_content_type (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return nullptr;
}

UShort TypeCodeBase::_s_fixed_digits (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

Short TypeCodeBase::_s_fixed_scale (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

Visibility TypeCodeBase::_s_member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

ValueModifier TypeCodeBase::_s_type_modifier (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	set_BadKind (_env);
	return nullptr;
}

Type <CORBA::Boolean>::ABI_ret TypeCodeORB::_s_equal (Bridge <CORBA::TypeCode>* _b, Interface* tc, Interface* _env) noexcept
{
	Bridge <ORB>* br_orb = orb_impl.operator -> ();
	return (br_orb->_epv ().epv.tc_equal) (br_orb, _b, tc, _env);
}

Type <CORBA::Boolean>::ABI_ret TypeCodeORB::_s_equivalent (Bridge <CORBA::TypeCode>* _b, Interface* tc, Interface* _env) noexcept
{
	Bridge <ORB>* br_orb = orb_impl.operator -> ();
	return (br_orb->_epv ().epv.tc_equivalent) (br_orb, _b, tc, _env);
}

Interface* TypeCodeORB::_s_get_compact_typecode (Bridge <CORBA::TypeCode>* _b, Interface* _env) noexcept
{
	Bridge <ORB>* br_orb = orb_impl.operator -> ();
	return (br_orb->_epv ().epv.get_compact_typecode) (br_orb, _b, _env);
}

Type <String>::ABI_ret TypeCodeStaticIdName::_s_id (Bridge <TypeCode>* _b, Interface* _env)
{
	return const_string_ret_p (((const StaticIdNameTC*)_b)->id);
}

Type <String>::ABI_ret TypeCodeStaticIdName::_s_name (Bridge <TypeCode>* _b, Interface* _env)
{
	return const_string_ret_p (((const StaticIdNameTC*)_b)->name);
}

ULong TypeCodeValueAbstractBase::_s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept
{
	return 0;
}

Type <String>::ABI_ret TypeCodeValueAbstractBase::_s_member_name (Bridge <TypeCode>* _b, ULong index,
	Interface* _env) noexcept
{
	set_Bounds (_env);
	return Type <String>::ret ();
}

Interface* TypeCodeValueAbstractBase::_s_member_type (Bridge <TypeCode>* _b, ULong index,
	Interface* _env) noexcept
{
	set_Bounds (_env);
	return nullptr;
}

Visibility TypeCodeValueAbstractBase::_s_member_visibility (Bridge <TypeCode>* _b, ULong index,
	Interface* _env) noexcept
{
	set_Bounds (_env);
	return 0;
}

ValueModifier TypeCodeValueAbstractBase::_s_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
noexcept
{
	return VM_ABSTRACT;
}

Interface* TypeCodeValueAbstractBase::_s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
noexcept
{
	return nullptr;
}

}
}
