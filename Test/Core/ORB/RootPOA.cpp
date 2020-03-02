#include "RootPOA.h"

using namespace PortableServer;

namespace CORBA {
namespace Nirvana {
namespace Core {

POA_var RootPOA::singleton_;

POA_var RootPOA::singleton ()
{
	if (!singleton_) {
		Servant_var <RootPOA> var = new RootPOA ();
		singleton_ = var->_this ();
	}
	return singleton_;
}

}
}
}
