#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {

Interface_ptr ObjectBase::_query_interface (const String& id)
{
	// Real implementation must return proxy
	// For test we just shortcut to servant
	return servant_->_query_interface (id);
}

}
}
