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
#include <CORBA/CORBA.h>
#include <CORBA/Proxy/TypeCodeImpl.h>
#include <string.h>

namespace CORBA {
namespace Nirvana {

void set_BadKind (Interface* env)
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::BadKind>::repository_id_, nullptr);
}

void set_Bounds (Interface* env)
{
	set_exception (env, Exception::EC_USER_EXCEPTION, RepIdOf <TypeCode::Bounds>::repository_id_, nullptr);
}

Boolean TypeCodeBase::equal (TCKind tk, TypeCode_ptr other)
{
	return tk == other->kind ();
}

Boolean TypeCodeBase::equivalent (TCKind tk, TypeCode_ptr other)
{
	TCKind tko = other->kind ();
	if (tk_alias == tko) {
		TypeCode_var type = TypeCode::_duplicate (other);
		do {
			type = type->content_type ();
			tko = type->kind ();
		} while (tk_alias == tko);
	}
	return tk == tko;
}

Boolean TypeCodeBase::equal (TCKind tk, const String& id, TypeCode_ptr other)
{
	if (!TypeCodeBase::equal (tk_except, other))
		return false;
	return id ==  other->id ();
}

Boolean TypeCodeBase::equivalent (TCKind tk, const String& id, TypeCode_ptr other)
{
	if (!TypeCodeBase::equivalent (tk_except, other))
		return false;
	return id == other->id ();
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

const Any* TypeCodeBase::_member_label (Bridge <TypeCode>* _b, ULong index, Interface* _env)
{
	set_BadKind (_env);
	return nullptr;
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
