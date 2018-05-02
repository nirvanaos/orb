#include "AbstractBase.h"

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p)
{
	if (!p)
		throw Exception (33);	// Invalid pointer
}

void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv)
{
	if (!obj || &obj->_epv () != &epv)
		throw Exception (33);	// Invalid pointer
}

}
}