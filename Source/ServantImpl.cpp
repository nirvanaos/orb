#include <CORBA/ServantImpl.h>
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv)
{
	_check_pointer (obj);
	if (&obj->_epv () != &epv)
		::Nirvana::throw_INV_OBJREF ();	// Invalid object pointer
}

}
}
