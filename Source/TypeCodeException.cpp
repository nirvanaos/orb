#include <CORBA/TypeCodeException.h>
#include <string.h>

namespace CORBA {
namespace Nirvana {

Boolean TypeCodeExceptionBase::equal (const char* id, TypeCode_ptr other)
{
	if (!TypeCodeBase::equal (tk_except, other))
		return false;
	return !strcmp (id, other->id ());
}

Boolean TypeCodeExceptionBase::equivalent (const char* id, TypeCode_ptr other)
{
	if (!TypeCodeBase::equivalent (tk_except, other))
		return false;
	return !strcmp (id, other->id ());
}

}
}

