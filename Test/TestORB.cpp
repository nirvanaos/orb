#include <CORBA/CORBA.h>
#include "I3.h"
#include <CORBA/PortableServer.h>

using namespace std;

/*
void must_not_compile (const I3_var& var)
{
	I1_var i1 = var; // Implicit cast between var must cause a compilation error
}
*/

namespace TestORB {

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
	CORBA::Nirvana::set_exception (eb, CORBA::NO_MEMORY ());
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
	set_exception (&ne, CORBA::NO_MEMORY ());
	const CORBA::Exception* ex = ne.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	CORBA::Nirvana::Environment ne1 (move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");

	CORBA::Nirvana::EnvironmentEx <TestException> nex;
	CORBA::Nirvana::set_exception (&nex, TestException ());
	CORBA::Nirvana::Environment ne2 (move (nex));
	ex = ne2.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), TestException::__name ());
}

TEST_F (TestORB, ServantTypes)
{
	{
		PortableServer::Servant servant = new DynamicI1 (1);
		PortableServer::release (servant);
	}
#ifndef TEST_NO_POA
	{
		PortableServer::Servant servant = new PortableI1 (1);
		PortableServer::release (servant);
	}
#endif
	{
		CORBA::LocalObject_ptr servant = new DynamicI3 (1);
		EXPECT_FALSE (servant->_non_existent ());
		CORBA::Object_ptr obj = servant;
		EXPECT_TRUE (obj);
		CORBA::release (servant);
	}
#ifndef TEST_NO_POA
	{
		CORBA::LocalObject_ptr servant = new PortableI3 (1);
		CORBA::release (servant);
	}
#endif
}

}
