#include <CORBA/ServantImpl.h>
#include <Nirvana/throw_exception.h>
#include <CORBA/Type_forward.h>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const Interface* obj, const Interface::EPV& epv)
{
	_check_pointer (obj);
	if (&obj->_epv () != &epv)
		::Nirvana::throw_INV_OBJREF ();	// Invalid object pointer
}

}
}
