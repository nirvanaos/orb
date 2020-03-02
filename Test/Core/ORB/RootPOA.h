#ifndef NIRVANA_ORB_CORE_ROOTPOA_H_
#define NIRVANA_ORB_CORE_ROOTPOA_H_

#include "ServantBase.h"
#include <CORBA/POA_s.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

class RootPOA :
	public Servant <RootPOA, PortableServer::POA>
{
public:
	~RootPOA ()
	{
		singleton_ = PortableServer::POA::_nil ();
	}

	static PortableServer::POA_var singleton ();

	std::string activate_object (PortableServer::Servant servant)
	{
		Bridge <PortableServer::ServantBase>* ps = servant->__core_servant ();
		ServantBase* psc = static_cast <ServantBase*> (ps);
		assert (!psc->is_active_);
		psc->is_active_ = true;
		return "Objectid";
	}

private:
	static PortableServer::POA_ptr singleton_;
};

}
}
}

#endif
