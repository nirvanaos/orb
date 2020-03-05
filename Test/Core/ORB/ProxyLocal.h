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
		::Nirvana::ContextFrame frame;
		sync_context ()->enter (frame);
		Boolean ret = servant_->_is_equivalent (other_object);
		sync_context ()->leave (frame);
		return ret;
	}

	ULong _hash (ULong maximum)
	{
		::Nirvana::ContextFrame frame;
		sync_context ()->enter (frame);
		ULong ret = servant_->_hash (maximum);
		sync_context ()->leave (frame);
		return ret;
	}

	// TODO: Other Object operations shall be here...

};

}
}
}

#endif
