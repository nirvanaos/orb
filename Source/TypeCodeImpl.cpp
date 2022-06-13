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
#include <CORBA/Proxy/TypeCodeImpl.h>

namespace CORBA {
namespace Internal {

void set_BadKind (Interface* env) NIRVANA_NOEXCEPT
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::BadKind>::repository_id_, nullptr);
}

void set_Bounds (Interface* env) NIRVANA_NOEXCEPT
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::Bounds>::repository_id_, nullptr);
}

I_ref <TypeCode> TypeCodeBase::dereference_alias (I_ptr <TypeCode> tc)
{
	I_ref <TypeCode> ret = tc;
	while (TCKind::tk_alias == ret->kind ()) {
		ret = ret->content_type ();
	}
	return ret;
}

Boolean TypeCodeBase::equal (TCKind tk, I_ptr <TypeCode> other)
{
	return tk == other->kind ();
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
	I_ref <TypeCode> tco = dereference_alias (other);
	return equal (tk, bound, tco) && content->equivalent (tco->content_type ());
}

Boolean TypeCodeBase::equal (TCKind tk, String_in& id, I_ptr <TypeCode> other)
{
	return equal (tk, other)
		&& static_cast <const String&> (id) == other->id ();
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in& id, I_ptr <TypeCode> other)
{
	return equal (tk, id, dereference_alias (other));
}

Boolean TypeCodeBase::equal (TCKind tk, String_in& id, String_in& name, I_ptr <TypeCode> other)
{
	if (!equal (tk, id, other))
		return false;

	return static_cast <const String&> (name) == other->name ();
}

Boolean TypeCodeBase::equal (TCKind tk, String_in& id, String_in& name,
	const Char* const* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	if (!equivalent_ (tk, id, member_cnt, other))
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (other->member_name (i) != members [i])
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equivalent_ (TCKind tk, String_in& id, ULong member_cnt, I_ptr <TypeCode> other)
{
	if (!equal (tk, id, other))
		return false;
	return other->member_count () == member_cnt;
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in& id, ULong member_cnt, I_ptr <TypeCode> other)
{
	return equivalent_ (tk, id, member_cnt, dereference_alias (other));
}

Boolean TypeCodeBase::equal (TCKind tk, String_in& id, String_in& name,
	const Parameter* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	if (!equal (tk, id, name, other))
		return false;

	if (other->member_count () != member_cnt)
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (other->member_name (i) != members [i].name)
			return false;
		if (!other->member_type (i)->equal ((members [i].type) ()))
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in& id,
	const Parameter* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	I_ref <TypeCode> tco = dereference_alias (other);
	if (!equivalent_ (tk, id, member_cnt, tco))
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (!tco->member_type (i)->equivalent ((members [i].type) ()))
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equal (String_in& id, String_in& name,
	ValueModifier mod, GetTypeCode base,
	const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	if (!equal (TCKind::tk_value, id, name, other))
		return false;

	if (!equal (mod, base, other))
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (other->member_name (i) != members [i].name)
			return false;
		if (!other->member_type (i)->equal ((members [i].type) ()))
			return false;
		if (other->member_visibility (i) != members [i].visibility)
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equivalent (String_in& id, ValueModifier mod, GetTypeCode base,
	const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	I_ref <TypeCode> tco = dereference_alias (other);

	if (!equivalent_ (TCKind::tk_value, id, member_cnt, tco))
		return false;

	if (!equal (mod, base, tco))
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (!other->member_type (i)->equivalent ((members [i].type) ()))
			return false;
		if (other->member_visibility (i) != members [i].visibility)
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equal (ValueModifier mod, GetTypeCode base, I_ptr <TypeCode> other)
{
	if (other->type_modifier () != mod)
		return false;

	I_ptr <TypeCode> other_base = other->concrete_base_type ();
	if (base) {
		I_ptr <TypeCode> tc_base ((base)());
		if (!other_base || !tc_base->equal (other_base))
			return false;
	} else if (other_base)
		return false;

	return true;
}

Type <String>::ABI_ret TypeCodeBase::_id (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Type <String>::ABI_ret TypeCodeBase::_name (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

ULong TypeCodeBase::_member_count (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Type <String>::ABI_ret TypeCodeBase::_member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Interface* TypeCodeBase::_member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Type <Any>::ABI_ret TypeCodeBase::_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return Type <Any>::ret ();
}

Interface* TypeCodeBase::_discriminator_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Long TypeCodeBase::_default_index (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

ULong TypeCodeBase::_length (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_content_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

UShort TypeCodeBase::_fixed_digits (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Short TypeCodeBase::_fixed_scale (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Visibility TypeCodeBase::_member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

ValueModifier TypeCodeBase::_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

}
}
