#include "ServantCore.h"
#include "POACore.h"

namespace CORBA {
namespace Nirvana {

::PortableServer::POA_ptr ServantCore::_default_POA () const
{
	return InterfaceStatic <POACore, PortableServer::POA>::_get_ptr ();
}

}
}
