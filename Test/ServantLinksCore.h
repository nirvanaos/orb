#ifndef NIRVANA_TESTORB_SERVANTLINKSCORE_H_
#define NIRVANA_TESTORB_SERVANTLINKSCORE_H_

#include "ServantCore.h"
#include "ObjectCore.h"

namespace CORBA {
namespace Nirvana {

class ServantLinksCore :
	public Implementation <ServantLinksCore>,
	public LifeCycleNoCopy <ServantLinksCore>,
	public InterfaceImpl <ServantLinksCore, ServantLinks>
{
public:
	ServantLinksCore (ServantBase_ptr servant, const Char* type_id) :
		servant_base_ (servant, type_id),
		object_ (servant)
	{}

	ServantBase_ptr servant_base ()
	{
		return &static_cast <Bridge <ServantBase>&> (servant_base_);
	}

	Bridge <Object>* object ()
	{
		return &object_;
	}

private:
	ServantCore servant_base_;
	ObjectCore object_;
};

}
}

#endif
