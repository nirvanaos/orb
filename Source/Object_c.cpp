#include <Object_c.h>
#include <RepositoryId.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge <Object>::interface_id_ [] = CORBA_REPOSITORY_ID (Object);

void test ()
{
	Object_ptr p;
	Object_ptr p1 = p->_query_interface <Object> ();
}

}
}