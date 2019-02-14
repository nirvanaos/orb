#ifndef NIRVANA_TESTORB_SERVANTLINKSCORE_H_
#define NIRVANA_TESTORB_SERVANTLINKSCORE_H_

#include "ServantCore.h"
#include "ObjectCore.h"

namespace CORBA {
namespace Nirvana {

class ServantLinksCore :
	public ServantLinks
{
public:
	ServantLinksCore (ServantBase_ptr servant, const Char* type_id) :
		servant_core_ (servant, type_id),
		object_core_ (servant)
	{
		object = &object_core_;
		servant_base = &servant_core_;
	}

private:
	ServantCore servant_core_;
	ObjectCore object_core_;
};

}
}

#endif
