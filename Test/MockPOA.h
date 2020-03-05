#ifndef NIRVANA_TEST_MOCKPOA_H_
#define NIRVANA_TEST_MOCKPOA_H_

#include "Core/ORB/POA.h"
#include <gtest/gtest.h>

namespace Nirvana {
namespace Test {

class MockPOA :
	public CORBA::Nirvana::Core::POA
{
public:
	MockPOA ()
	{
		CORBA::Nirvana::Core::g_root_POA = _this ();
	}

	~MockPOA ()
	{
		EXPECT_EQ (_refcount_value (), 1);
		CORBA::Nirvana::Core::g_root_POA = PortableServer::POA::_nil ();
	}
};

}
}

#endif
