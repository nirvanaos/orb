#include "Interface_c.h"

namespace CORBA {
namespace Nirvana {

Interface_ptr Interface::_duplicate (Interface_ptr itf)
{
	if (!is_nil (itf)) {
		Environment env;
		itf = (itf->_epv ().duplicate) (itf, &env);
		env.check ();
	}
	return itf;
}

void Interface::_release (Interface_ptr itf)
{
	if (!is_nil (itf))
		(itf->_epv ().release) (itf);
}

}
}
	