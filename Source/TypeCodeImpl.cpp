#include <CORBA/TypeCodeImpl.h>

namespace CORBA {
namespace Nirvana {

NIRVANA_NORETURN void TypeCodeBase::throw_BadKind ()
{
	throw TypeCode::BadKind ();
}

NIRVANA_NORETURN void TypeCodeBase::throw_Bounds ()
{
	throw TypeCode::Bounds ();
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
