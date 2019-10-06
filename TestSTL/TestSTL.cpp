#include <gtest/gtest.h>
#include <CORBA/String.h>
#include <Mock/MockMemory.h>

namespace Nirvana {
Memory_ptr g_default_heap;
}

namespace TestSTL {

using namespace std;

class TestSTL :
	public ::testing::Test
{
protected:
	TestSTL ()
	{}

	virtual ~TestSTL ()
	{}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp ()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		Nirvana::g_default_heap = Nirvana::Test::mock_memory ();
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
};

TEST_F (TestSTL, String)
{}

}

