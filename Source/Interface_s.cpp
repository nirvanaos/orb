#include <CORBA/Interface_s.h>
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p)
{
	if (!p)
		::Nirvana::throw_BAD_PARAM ();	// Invalid pointer
}

void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv)
{
	if (!obj || &obj->_epv () != &epv)
		::Nirvana::throw_INV_OBJREF ();	// Invalid pointer
}

}
}
