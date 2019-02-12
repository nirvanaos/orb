#include "AbstractBase_s.h"

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p)
{
	if (!p)
		throw BAD_PARAM ();	// Invalid pointer
}

void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv)
{
	if (!obj || &obj->_epv () != &epv)
		throw INV_OBJREF ();	// Invalid pointer
}

}
}