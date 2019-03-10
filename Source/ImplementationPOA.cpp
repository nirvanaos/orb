#include <ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

inline void ServantPOA <ServantBase>::_final_construct ()
{
	if (!servant_links_)
		ServantBaseLinks::_final_construct (_primary_interface ());
}

void ServantPOA <ServantBase>::_implicitly_activate ()
{
	_final_construct ();
	ServantBaseLinks::_implicitly_activate ();
}

ServantPOA <ServantBase>::operator Bridge <Object>& ()
{
	_final_construct ();
	return ServantBaseLinks::operator Bridge <Object>& ();
}

ServantPOA <ServantBase>::operator ServantLinks_ptr ()
{
	_final_construct ();
	return ServantBaseLinks::operator ServantLinks_ptr ();
}

}
}