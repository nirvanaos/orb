#include <ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

void ServantPOA <ServantBase>::_final_construct ()
{
	ServantBaseLinks::_final_construct (_primary_interface ());
}

void ServantPOA <ServantBase>::_implicitly_activate ()
{
	_check_links ();
	ServantBaseLinks::_implicitly_activate ();
}

ServantPOA <ServantBase>::operator Bridge <Object>& ()
{
	_check_links ();
	return ServantBaseLinks::operator Bridge <Object>& ();
}

}
}