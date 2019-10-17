#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

Boolean ObjectBase::_is_a (const Char* type_id) const
{
	Bridge <Interface>* itf = servant_->_query_interface (type_id);
	if (itf)
		return TRUE;
	else
		return FALSE;
}

Interface_ptr ObjectBase::_query_interface (const Char* id)
{
	// Real implementation must return proxy
	// For test we just shortcut to servant
	return Interface::unmarshal (servant_->_query_interface (id));
}

}
}
