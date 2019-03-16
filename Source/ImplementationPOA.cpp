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

Interface_ptr ServantPOA <ServantBase>::_query_interface (const Char* id)
{
	return Interface::_duplicate (FindInterface <ServantBase /*, ::CORBA::Object*/>::find (*this, id));
}

ServantPOA <Object>::operator Bridge <Object>& ()
{
	_implicitly_activate ();
	return *servant_links_->object ();
}

Interface_ptr ServantPOA <Object>::_query_interface (const Char* id)
{
	return Interface::_duplicate (FindInterface <::CORBA::Object>::find (*this, id));
}

}
}