#ifndef NIRVANA_TESTORB_ROOTPOA_H_
#define NIRVANA_TESTORB_ROOTPOA_H_

#include "Core/ORB/POA.h"

namespace TestORB {

class RootPOA :
	public CORBA::Nirvana::Core::POA
{
public:
	static void create ();
	static void destroy ();

	~RootPOA ()
	{
		CORBA::Nirvana::Core::g_root_POA = PortableServer::POA::_nil ();
	}
};

}

#endif
