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
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::BadKind>::id, nullptr);
}

void set_Bounds (Interface* env) NIRVANA_NOEXCEPT
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::Bounds>::id, nullptr);
}

I_ptr <TypeCode> TypeCodeBase::dereference_alias (I_ptr <TypeCode> tc)
{
	while (tc && TCKind::tk_alias == tc->kind ()) {
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

Boolean TypeCodeBase::equal (TCKind tk, String_in& id, String_in& name, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	return equal (tk, id, name, other) && content->equal (other->content_type ());
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in& id, I_ptr <TypeCode> content, I_ptr <TypeCode> other)
{
	EqResult eq = equivalent_ (tk, id, other);
	if (EqResult::UNKNOWN != eq)
		return EqResult::NO != eq;
	return content->equal (other->content_type ());
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

TypeCodeBase::EqResult TypeCodeBase::equivalent_ (TCKind tk, String_in& id, I_ptr <TypeCode> other)
{
	if (!equal (tk, other))
		return EqResult::NO;
	const String& oid = other->id ();
	if (!id.empty () && !oid.empty ())
		return static_cast <const String&> (id) == oid ? EqResult::YES : EqResult::NO;
	return EqResult::UNKNOWN;
}

Boolean TypeCodeBase::equivalent (TCKind tk, String_in& id, ULong member_cnt, I_ptr <TypeCode> other)
{
	I_ptr <TypeCode> tco = dereference_alias (other);
	EqResult eq = equivalent_ (tk, id, tco);
	if (EqResult::UNKNOWN != eq)
		return EqResult::YES == eq;
	return member_cnt == tco->member_count ();
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
	I_ptr <TypeCode> tco = dereference_alias (other);
	EqResult eq = equivalent_ (tk, id, tco);
	if (EqResult::UNKNOWN != eq)
		return EqResult::YES == eq;

	if (other->member_count () != member_cnt)
		return false;

	for (ULong i = 0; i < member_cnt; ++i) {
		if (!tco->member_type (i)->equivalent ((members [i].type) ()))
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equal (Bridge <TypeCode>* bridge, String_in& id, String_in& name,
	ValueModifier mod, GetTypeCode base,
	const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other)
{
#ifndef _DEBUG
	if (bridge == &other)
		return true;
#endif

	if (!equal (TCKind::tk_value, id, name, other))
		return false;

	if (other->type_modifier () != mod)
		return false;

	I_ptr <TypeCode> other_base = other->concrete_base_type ();
	if (base) {
		I_ptr <TypeCode> tc_base ((base)());
		if (!other_base || !tc_base->equal (other_base))
			return false;
	} else if (other_base)
		return false;

	if (other->member_count () != member_cnt)
		return false;

	CompareSet cs (bridge, &other);

	for (ULong i = 0; i < member_cnt; ++i) {
		if (other->member_name (i) != members [i].name)
			return false;
		if (!equal (cs, (members [i].type) (), other->member_type (i)))
			return false;
		if (other->member_visibility (i) != members [i].visibility)
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equal (CompareSet& cs,
	I_ptr <TypeCode> left, I_ptr <TypeCode> right)
{
#ifndef _DEBUG
	if (&left == &right)
		return true;
#endif

	// If this is a value type with members, we prevent the recursion
	if (left->kind () == TCKind::tk_value) {
		ULong member_cnt = left->member_count ();
		if (member_cnt > 0) {
			if (cs.insert (&left, &right)) {
				if (right->kind () != TCKind::tk_value)
					return false;
				if (right->member_count () != member_cnt)
					return false;
				if (left->id () != right->id ())
					return false;
				if (left->name () != right->name ())
					return false;

				I_ptr <TypeCode> lb = left->concrete_base_type (),
					rb = right->concrete_base_type ();

				if (lb) {
					if (!rb || !lb->equal (rb))
						return false;
				} else if (rb)
					return false;

				for (ULong i = 0; i < member_cnt; ++i) {
					if (left->member_name (i) != right->member_name (i))
						return false;
					if (!equal (cs, left->member_type (i), right->member_type (i)))
						return false;
					if (left->member_visibility (i) != right->member_visibility (i))
						return false;
				}
			}
			return true;
		}
	}

	// Otherwise as usual
	return left->equal (right);
}

Boolean TypeCodeBase::equivalent (Bridge <TypeCode>* bridge, String_in& id,
	ValueModifier mod, GetTypeCode base,
	const StateMember* members, ULong member_cnt, I_ptr <TypeCode> other)
{
	I_ptr <TypeCode> tco = dereference_alias (other);

#ifndef _DEBUG
	if (bridge == &tco)
		return true;
#endif

	EqResult eq = equivalent_ (TCKind::tk_value, id, tco);
	if (EqResult::UNKNOWN != eq)
		return EqResult::YES == eq;

	I_ptr <TypeCode> other_base = tco->concrete_base_type ();
	if (base) {
		I_ptr <TypeCode> tc_base ((base)());
		if (!other_base || !tc_base->equivalent (other_base))
			return false;
	} else if (other_base)
		return false;

	if (tco->member_count () != member_cnt)
		return false;

	CompareSet cs (bridge, &tco);

	for (ULong i = 0; i < member_cnt; ++i) {
		if (!equivalent (cs, (members [i].type) (), tco->member_type (i)))
			return false;
		if (tco->member_visibility (i) != members [i].visibility)
			return false;
	}
	return true;
}

Boolean TypeCodeBase::equivalent (CompareSet& cs,
	I_ptr <TypeCode> l, I_ptr <TypeCode> r)
{
	I_ptr <TypeCode> left = dereference_alias (l);
	I_ptr <TypeCode> right = dereference_alias (r);

#ifndef _DEBUG
	if (&left == &right)
		return true;
#endif

	// If this is a value type with members, we prevent the recursion
	if (left->kind () == TCKind::tk_value) {
		ULong member_cnt = left->member_count ();
		if (member_cnt > 0) {
			if (cs.insert (&left, &right)) {
				if (right->kind () != TCKind::tk_value)
					return false;

				String lid = left->id (), rid = right->id ();
				if (!lid.empty () && !rid.empty ())
					return lid == rid;

				if (right->member_count () != member_cnt)
					return false;

				I_ptr <TypeCode> lb = left->concrete_base_type (),
					rb = right->concrete_base_type ();

				if (lb) {
					if (!rb || !lb->equal (rb))
						return false;
				} else if (rb)
					return false;

				for (ULong i = 0; i < member_cnt; ++i) {
					if (!equivalent (cs, left->member_type (i), right->member_type (i)))
						return false;
					if (left->member_visibility (i) != right->member_visibility (i))
						return false;
				}
			}
			return true;
		}
	}

	// Otherwise as usual
	return left->equivalent (right);
}

Type <String>::ABI_ret TypeCodeBase::_s_id (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Type <String>::ABI_ret TypeCodeBase::_s_name (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

ULong TypeCodeBase::_s_member_count (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Type <String>::ABI_ret TypeCodeBase::_s_member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return Type <String>::ret ();
}

Interface* TypeCodeBase::_s_member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Type <Any>::ABI_ret TypeCodeBase::_s_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return Type <Any>::ret ();
}

Interface* TypeCodeBase::_s_discriminator_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Long TypeCodeBase::_s_default_index (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

ULong TypeCodeBase::_s_length (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_s_content_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

UShort TypeCodeBase::_s_fixed_digits (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Short TypeCodeBase::_s_fixed_scale (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Visibility TypeCodeBase::_s_member_visibility (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

ValueModifier TypeCodeBase::_s_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return 0;
}

Interface* TypeCodeBase::_s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
}

}
}
