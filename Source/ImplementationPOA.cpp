#include <ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

Bridge <Interface>* InterfaceEntryPOA::find (const InterfaceEntryPOA* begin, const InterfaceEntryPOA* end, void* servant, const Char* id)
{
	for (const InterfaceEntryPOA* p = begin; p != end; ++p) {
		Bridge <Interface>* f = (p->find_base) (servant, id);
		if (f)
			return f;
	}
	return nullptr;
}

Interface_ptr ServantPOA <AbstractBase>::_query_interface (const Char* id)
{
	return Interface_ptr::nil ();
}

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
	return InterfaceFinderPOA <ServantBase>::find (*this, id);
}

ServantPOA <Object>::operator Bridge <Object>& ()
{
	_implicitly_activate ();
	return *servant_links_->object ();
}

Interface_ptr ServantPOA <Object>::_query_interface (const Char* id)
{
	return InterfaceFinderPOA <Object>::find (*this, id);
}

}
}