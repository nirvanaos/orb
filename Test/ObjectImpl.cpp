#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

Boolean ObjectBase::_is_a (const Char* type_id) const
{
	AbstractBase_ptr base = servant_;
	Bridge <Interface>* itf = base->_query_interface (type_id);
	if (itf)
		return TRUE;
	else
		return FALSE;
}

}
}
