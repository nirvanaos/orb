#include "Test_I1.h"
#include <gtest/gtest.h>

using namespace std;
using namespace CORBA;
using namespace Test;

namespace unittests {

// The fixture for testing BPMXConverterBase functions.
class TestORB :
	public ::testing::Test
{
protected:
	TestORB ()
	{}

	virtual ~TestORB ()
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
		ASSERT_EQ (sm_inst_cnt, 0);
	}

	// Objects declared here can be used by all tests in the test case.
	static void test (I1_ptr, Long val);

	class Instance
	{
	protected:
		Instance ()
		{
			++sm_inst_cnt;
		}

		~Instance ()
		{
			--sm_inst_cnt;
		}
	};

	static int sm_inst_cnt;
};

int TestORB::sm_inst_cnt = 0;

void TestORB::test (I1_ptr p, Long val)
{
	ASSERT_FALSE (is_nil (p));
	Object_ptr object = p;
	ASSERT_FALSE (is_nil (object));
	AbstractBase_ptr ab = object;
	ASSERT_FALSE (is_nil (ab));
	Object_ptr o1 = ab->_to_object ();
	ASSERT_FALSE (is_nil (o1));
	I1_ptr p1 = I1::_narrow (object);
	ASSERT_FALSE (is_nil (p1));
	release (p1);
	ASSERT_FALSE (p->_non_existent ());
	ASSERT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	ASSERT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));
	Long res = p->op1 (1);
	ASSERT_EQ (res, val + 1);
	release (p);
}

TEST_F (TestORB, Dynamic)
{
	// Dynamic implementation
	class Servant :
		public ::PortableServer::Nirvana::Servant <Servant, I1>,
		public Instance
	{
	public:
		Servant (Long addendum) :
			m_addendum (addendum)
		{}

		Long op1 (Long p1) const
		{
			return p1 + m_addendum;
		}

	private:
		Long m_addendum;
	};

	Servant* servant = new Servant (2);
	test (servant->_this (), 2);
}

TEST_F (TestORB, Portable)
{
	// Portable implementation
	class Servant :
		public POA_I1,
		public Instance
	{
	public:
		Servant (Long addendum) :
			m_addendum (addendum)
		{}

		virtual Long op1 (Long p1)
		{
			return p1 + m_addendum;
		}

	private:
		Long m_addendum;
	};

	Servant* servant = new Servant (2);
	test (servant->_this (), 2);
}

}
