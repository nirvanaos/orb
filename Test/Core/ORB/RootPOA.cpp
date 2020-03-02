#include "RootPOA.h"

using namespace PortableServer;

namespace CORBA {
namespace Nirvana {
namespace Core {

POA_ptr RootPOA::singleton_;

POA_var RootPOA::singleton ()
{
	if (!singleton_) {
		Servant_var <RootPOA> var = new RootPOA ();
		return singleton_ = var->_this ();
	} else
		return POA::_duplicate (singleton_);
}

}
}
}
