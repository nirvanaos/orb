#ifndef NIRVANA_ORB_CORE_ROOTPOA_H_
#define NIRVANA_ORB_CORE_ROOTPOA_H_

#include "../StaticObject.h"
#include "ServantBase.h"
#include <CORBA/POA_s.h>
#include <CORBA/ImplementationPseudoStatic.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

class RootPOA :
	public ImplementationPseudoStatic <RootPOA, PortableServer::POA>,
	public StaticObject <RootPOA>
{
public:
	static Interface_ptr _query_interface (const std::string& iid)
	{
		return FindInterface <PortableServer::POA, Object>::find (*(RootPOA*)0, iid);
	}

	std::string activate_object (PortableServer::Servant servant)
	{
		Bridge <PortableServer::ServantBase>* ps = servant->__core_servant ();
		ServantBase* psc = static_cast <ServantBase*> (ps);
		assert (!psc->is_active_);
		psc->is_active_ = true;
		return "Objectid";
	}
};

const StaticI_ptr <PortableServer::POA> g_root_poa = { STATIC_BRIDGE (RootPOA, PortableServer::POA) };

}
}
}

#endif
