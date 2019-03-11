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

Interface_ptr ServantPOA <ServantBase>::_query_interface (const Char* id)
{
	return Interface::_duplicate (FindInterface <Object>::find (*this, id));
}

Bridge <Interface>* InterfaceEntryPOA::find (const InterfaceEntryPOA* begin, const InterfaceEntryPOA* end, void* servant, const Char* id) 
{
	for (const InterfaceEntryPOA* p = begin; p != end; ++p) {
		Bridge <Interface>* f = (p->find_base) (servant, id);
		if (f)
			return f;
	}
	return nullptr;
}

}
}