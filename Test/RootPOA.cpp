#include "RootPOA.h"
#include <gtest/gtest.h>

namespace TestORB {

void RootPOA::create ()
{
	CORBA::Nirvana::Core::g_root_POA = PortableServer::Servant_var <RootPOA> (new RootPOA ())->_this ();
}

void RootPOA::destroy ()
{
	CORBA::release (PortableServer::POA_ptr (CORBA::Nirvana::Core::g_root_POA));
	EXPECT_TRUE (CORBA::is_nil (PortableServer::POA_ptr (CORBA::Nirvana::Core::g_root_POA)));
}

}
