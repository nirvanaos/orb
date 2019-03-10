#ifndef NIRVANA_TESTORB_POACORE_H_
#define NIRVANA_TESTORB_POACORE_H_

#include "LocalObjectStaticCore.h"
#include "ServantLinksCore.h"
#include <POA_s.h>

namespace CORBA {
namespace Nirvana {

class POACore :
	public LocalObjectStaticCore <POACore, ::PortableServer::POA>,
	public InterfaceStatic <POACore, ::PortableServer::POA>
{
public:
	const Char* activate_object (Bridge <ServantLinks>* servant)
	{
		ServantLinksCore* links = static_cast <ServantLinksCore*> (servant);
		assert (!links->is_active_);
		links->is_active_ = TRUE;
		return "Objectid";
	}
};

}
}

#endif

