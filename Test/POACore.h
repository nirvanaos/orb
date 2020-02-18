#ifndef NIRVANA_TESTORB_POACORE_H_
#define NIRVANA_TESTORB_POACORE_H_

#include "LocalObjectStaticCore.h"
#include "ObjectCore.h"
#include <CORBA/POA_s.h>

namespace CORBA {
namespace Nirvana {

class POACore :
	public ImplementationStaticPseudo <POACore, PortableServer::POA>,
	public LocalObjectStaticCore <POACore>
{
public:
	static Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <PortableServer::POA, Object>::find (*(POACore*)0, id);
	}

	const Char* activate_object (PortableServer::Servant servant)
	{
		Bridge <Object>* p = Object_ptr (servant);
		ObjectCore* obj = static_cast <ObjectCore*> (p);
		assert (!obj->is_active_);
		obj->is_active_ = true;
		return "Objectid";
	}
};

}
}

#endif

