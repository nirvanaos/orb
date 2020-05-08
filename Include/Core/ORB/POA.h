#ifndef NIRVANA_ORB_CORE_POA_H_
#define NIRVANA_ORB_CORE_POA_H_

#include "ServantBase.h"
#include <CORBA/POA_s.h>
#include <unordered_map>

namespace CORBA {
namespace Nirvana {
namespace Core {

extern StaticI_ptr <PortableServer::POA> g_root_POA; // Temporary solution

class POA :
	public Servant <POA, PortableServer::POA>
{
public:
	String activate_object (PortableServer::Servant p_servant)
	{
		PortableServer::Servant ps = p_servant->__core_servant ();
		ServantBase* core_obj = static_cast <ServantBase*> (&ps);
		Object_ptr proxy = core_obj->get_proxy ();
		std::pair <AOM::iterator, bool> ins = active_object_map_.emplace (std::to_string ((uintptr_t)&proxy), 
			Object_var (Object::_duplicate (proxy)));
		if (!ins.second)
			throw PortableServer::POA::ServantAlreadyActive ();
		return ins.first->first;
	}

	void deactivate_object (const String& oid)
	{
		if (!active_object_map_.erase (oid))
			throw PortableServer::POA::ObjectNotActive ();
	}

private:
	typedef std::unordered_map <String, Object_var> AOM;
	AOM active_object_map_;
};

}
}
}

#endif
