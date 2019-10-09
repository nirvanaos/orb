#include <Nirvana/basic_string.h>
#include <gtest/gtest.h>
#include <CORBA/ObjectFactory_c.h>
#include <Mock/MockMemory.h>

namespace CORBA {
namespace Nirvana {
Bridge <ObjectFactory>* const ObjectFactory::singleton_ = nullptr;
}
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
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
};

TEST_F (TestSTL, String)
{
	std::basic_string <char, std::char_traits<char>, std::allocator <char> > s ("small");
	s = "Large large large large string";
}

}

