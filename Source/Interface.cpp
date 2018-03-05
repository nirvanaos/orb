#include "Interface.h"

namespace CORBA {
namespace Nirvana {

Bridge <Interface>* Interface::__duplicate (Bridge <Interface>* p)
{
	if (p) {
		Environment env;
		p = (p->_epv ().duplicate) (p, &env);
		env.check ();
	}
	return p;
}

void Interface::__release (Bridge <Interface>* p)
{
	if (p)
		(p->_epv ().release) (p);
}

}
}
	