#include <CORBA/TypeCodeImpl.h>

namespace CORBA {
namespace Nirvana {

void TypeCodeBase::set_BadKind (EnvironmentBridge* env)
{
	env->set_exception (-1, TypeCode::BadKind::repository_id_, nullptr);
}

void TypeCodeBase::set_Bounds (EnvironmentBridge* env)
{
	env->set_exception (-1, TypeCode::Bounds::repository_id_, nullptr);
}

void TypeCodeBase::set_BAD_TYPECODE (EnvironmentBridge* env)
{
	env->set_exception (BAD_TYPECODE ());
}

const char* TypeCodeBase::_id (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

const char* TypeCodeBase::_name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

ULong TypeCodeBase::_member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

const char* TypeCodeBase::_member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

BridgeMarshal <TypeCode>* TypeCodeBase::_member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Any* TypeCodeBase::_member_label (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

BridgeMarshal <TypeCode>* TypeCodeBase::_discriminator_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

Long TypeCodeBase::_default_index (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

ULong TypeCodeBase::_length (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

BridgeMarshal <TypeCode>* TypeCodeBase::_content_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
}

UShort TypeCodeBase::_fixed_digits (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

Short TypeCodeBase::_fixed_scale (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

Visibility TypeCodeBase::_member_visibility (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

ValueModifier TypeCodeBase::_type_modifier (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return 0;
}

BridgeMarshal <TypeCode>* TypeCodeBase::_concrete_base_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
{
	set_BadKind (_env);
	return nullptr;
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

}
}
