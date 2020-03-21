#ifndef NIRVANA_ORB_CORE_PROXYLOCAL_H_
#define NIRVANA_ORB_CORE_PROXYLOCAL_H_

#include "ProxyBase.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyLocal :
	public ProxyBase <ProxyLocal, Object>
{
public:
	ProxyLocal (Object_ptr servant, Interface_ptr lifecycle) :
		ProxyBase (servant, lifecycle)
	{}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		Object_var _tmp_other_object (Object::_duplicate (other_object));
		::Nirvana::Synchronized sync (sync_context ());
		return servant_->_is_equivalent (_tmp_other_object);
	}

	ULong _hash (ULong maximum)
	{
		::Nirvana::Synchronized sync (sync_context ());
		return servant_->_hash (maximum);
	}

	// TODO: Other Object operations shall be here...

};

}
}
}

#endif
