#ifndef NIRVANA_TESTORB_SERVANTLINKSCORE_H_
#define NIRVANA_TESTORB_SERVANTLINKSCORE_H_

#include "ServantCore.h"
#include "ObjectCore.h"
#include <ServantLinks_s.h>

namespace CORBA {
namespace Nirvana {

class ServantLinksCore :
	public ServantTraits <ServantLinksCore>,
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

	Object_ptr object ()
	{
		return &object_;
	}

	Boolean is_active () const
	{
		return is_active_;
	}

	ServantCore servant_base_;
	ObjectCore object_;
	Boolean is_active_;
};

}
}

#endif
