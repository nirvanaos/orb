#include "ServantBase.h"
#include "RootPOA.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

::PortableServer::POA_var ServantBase::_default_POA () const
{
	return RootPOA::singleton ();
}

}
}
}
