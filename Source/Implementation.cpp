#include <Implementation.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLinks::_final_construct (const Char* primary_interface)
{
	assert (is_nil (servant_links_));
	servant_links_ = g_object_adapter->create_servant (this, primary_interface);
}

void ServantBaseLinks::_implicitly_activate ()
{
	if (!servant_links_->is_active ()) {
		POA_var poa = ServantBase_ptr (this)->_default_POA ();
		poa->activate_object (*this);
	}
}

}
}