#include <ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

Interface_ptr ServantPOA <Object>::_query_interface (const Char* id)
{
	return Interface::_duplicate (FindInterface <::CORBA::Object>::find (*this, id));
}

}
}