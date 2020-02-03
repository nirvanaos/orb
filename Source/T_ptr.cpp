#include <CORBA/T_ptr.h>
#include <CORBA/Exception.h>

namespace CORBA {
namespace Nirvana {

NIRVANA_NORETURN void PtrExceptions::throw_INV_OBJREF ()
{
	throw INV_OBJREF ();
}

}
}
