#ifndef NIRVANA_ORB_TEST_SERVANTLINKSCORE_H_
#define NIRVANA_ORB_TEST_SERVANTLINKSCORE_H_

#include "ServantCore.h"

namespace CORBA {
namespace Nirvana {

class ServantLinksCore :
	public ServantLinks
{
public:
	ServantLinksCore (ServantBase_ptr servant, const Char* type_id) :
		servant_ (servant),
		servant_core_ (servant, type_id)
	{
		object = nullptr;
		servant_base = &servant_core_;
	}

	ServantBase_ptr servant () const
	{
		return servant_;
	}

private:
	ServantBase_ptr servant_;
	ServantCore servant_core_;
};

}
}

#endif
