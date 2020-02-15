#include <CORBA/CORBA.h>
#include <gtest/gtest.h>

using namespace std;

namespace TestORB {

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
	}
};

TEST_F (TestORB, RepositoryId)
{
	EXPECT_TRUE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.0"));
	EXPECT_TRUE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.1", "IDL:aaa/bbb/type:1.0"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.1"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/other:1.0"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "aaa/bbb/type:1.0"));
}

class TestException : public CORBA::UserException
{
public:
	DECLARE_EXCEPTION (TestException);

	TestException ()
	{}

	TestException (const Data*)
	{}
};

DEFINE_USER_EXCEPTION (TestException, "IDL:TestORB/TestException:1.0")

TEST_F (TestORB, SystemException)
{
	CORBA::NO_MEMORY nm;
	EXPECT_THROW (nm._raise (), CORBA::NO_MEMORY);
}

TEST_F (TestORB, UserException)
{
	TestException e;
	EXPECT_THROW (e._raise (), TestException);

	CORBA::Exception* copy = e.__clone ();
	EXPECT_THROW (copy->_raise (), TestException);
	delete copy;
}

TEST_F (TestORB, CORBA_Environment)
{
	CORBA::Environment_ptr env;
	CORBA::ORB::create_environment (env);
	CORBA::Nirvana::EnvironmentBridge* eb = env;
	eb->set_exception (CORBA::NO_MEMORY ());
	const CORBA::Exception* ex = env->exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	env->clear ();
	CORBA::Environment_var ev = env;
	CORBA::ORB::create_environment (ev);
	EXPECT_FALSE (ev->exception ());
}

TEST_F (TestORB, Environment)
{
	CORBA::Nirvana::Environment ne;
	ne.set (&CORBA::NO_MEMORY ());
	const CORBA::Exception* ex = ne.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	CORBA::Nirvana::Environment ne1 (move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");

	CORBA::Nirvana::EnvironmentEx <TestException> nex;
	nex.set (&TestException ());
	CORBA::Nirvana::Environment ne2 (move (nex));
	ex = ne2.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), TestException::__name ());
}

}
