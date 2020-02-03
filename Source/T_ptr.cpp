#include <CORBA/T_ptr.h>
#include <CORBA/Exception.h>

namespace CORBA {
namespace Nirvana {

NIRVANA_NORETURN void throw_INV_OBJREF ()
{
	throw INV_OBJREF ();
}

NIRVANA_NORETURN void throw_MARSHAL ()
{
	throw MARSHAL ();
}

}
}
