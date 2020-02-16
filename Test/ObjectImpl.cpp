#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

Interface_ptr ObjectBase::_query_interface (const Char* id)
{
	// Real implementation must return proxy
	// For test we just shortcut to servant
	return Interface::_check (servant_->_query_interface (id));
}

}
}
