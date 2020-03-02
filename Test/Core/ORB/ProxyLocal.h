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
	ProxyLocal (Object_ptr servant) :
		ProxyBase (servant)
	{}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		::Nirvana::ContextFrame frame;
		sync_domain ()->enter (frame);
		Boolean ret = servant_->_is_equivalent (other_object);
		sync_domain ()->leave (frame);
		return ret;
	}

	ULong _hash (ULong maximum)
	{
		::Nirvana::ContextFrame frame;
		sync_domain ()->enter (frame);
		ULong ret = servant_->_hash (maximum);
		sync_domain ()->leave (frame);
		return ret;
	}

	// TODO: Other Object operations shall be here...

};

}
}
}

#endif
