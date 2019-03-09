#include "I1.h"
#include "I3.h"
#include <gtest/gtest.h>

using namespace std;
using namespace CORBA;
using namespace Test;

// Imitation of the module loader
namespace CORBA {
namespace Nirvana {

#ifndef TEST_NO_STATIC

template <> const ServantLinks* StaticObject <TestORB::StaticI1, Test::I1>::servant_links_;
template <> const ServantLinks* StaticObject <TestORB::StaticI3, Test::I3>::servant_links_;

class ModuleLoader
{
public:
	ModuleLoader ()
	{
		load_static <TestORB::StaticI1, Test::I1> ();
		load_static <TestORB::StaticI3, Test::I3> ();
	}

	~ModuleLoader ()
	{
		unload_static <TestORB::StaticI1, Test::I1> ();
		unload_static <TestORB::StaticI3, Test::I3> ();
	}

private:
	void load_static (const OLF_ObjectInfo& oi, const ServantLinks*& sl)
	{
		sl = g_object_adapter->create_servant (oi.servant, oi.primary_interface);
	}

	void unload_static (const ServantLinks* sl)
	{
		g_object_adapter->destroy_servant (const_cast <ServantLinks*> (sl));
	}

	template <class S, class I>
	void load_static ()
	{
		load_static (StaticObject <TestORB::StaticI1, Test::I1>::object_info_, StaticObject <TestORB::StaticI1, Test::I1>::servant_links_);
	}

	template <class S, class I>
	void unload_static ()
	{
		unload_static (StaticObject <TestORB::StaticI1, Test::I1>::servant_links_);
	}
};

#endif

}
}

namespace TestORB {

int Instance::count_ = 0;

void test_interface (I1_ptr p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	Object_ptr object = p;
	ASSERT_FALSE (is_nil (object));
	ASSERT_FALSE (object->_non_existent ());
	AbstractBase_ptr ab = object;
	ASSERT_FALSE (is_nil (ab));
	{
		Object_ptr o1 = ab->_to_object ();
		ASSERT_FALSE (is_nil (o1));
		ASSERT_FALSE (o1->_non_existent ());
		release (o1);
	}
	{
		I1_ptr p1 = I1::_narrow (object);
		ASSERT_FALSE (is_nil (p1));
		ASSERT_FALSE (p1->_non_existent ());
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		release (p1);
	}
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	EXPECT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));
	release (p);
}

void test_performance (I1_ptr p)
{
	for (int i = 0; i < 1000000; ++i)
		p->op1 (2);
	release (p);
}

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

// The fixture for testing simple interface.

typedef ::testing::Types <DynamicI1
#ifndef TEST_NO_POA
	,PortableI1
#endif
#ifndef TEST_NO_STATIC
	,StaticI1
#endif
#ifndef TEST_NO_TIED
	,TiedI1
	,TiedDerivedI1
#endif
> ServantTypesI1;

template <class Servant>
class TestORB_I1 :
	public ::testing::Test
{
protected:
	TestORB_I1 ()
	{}

	virtual ~TestORB_I1 ()
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
		ASSERT_EQ (Instance::count (), 0);
	}
};

TYPED_TEST_CASE (TestORB_I1, ServantTypesI1);

TYPED_TEST (TestORB_I1, Inteface)
{
	test_interface (TypeParam::incarnate ());
}

TYPED_TEST (TestORB_I1, Performance)
{
	test_performance (TypeParam::incarnate ());
}

TYPED_TEST (TestORB_I1, Exception)
{
	I1_ptr p = TypeParam::incarnate ();
	EXPECT_THROW (p->throw_NO_IMPLEMENT (), NO_IMPLEMENT);
	release (p);
}

// The fixture for testing complex interface.

typedef ::testing::Types <DynamicI3
#ifndef TEST_NO_POA
	,PortableI3
#endif
#ifndef TEST_NO_STATIC
	,StaticI3
#endif
#ifndef TEST_NO_TIED
	,TiedI3
	,TiedDerivedI3
#endif
> ServantTypesI3;

template <class Servant>
class TestORB_I3 :
	public ::testing::Test
{
protected:
	TestORB_I3 ()
	{}

	virtual ~TestORB_I3 ()
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
		EXPECT_EQ (Instance::count (), 0);
	}
};

TYPED_TEST_CASE (TestORB_I3, ServantTypesI3);

TYPED_TEST (TestORB_I3, Inteface)
{
	test_interface (TypeParam::incarnate ());
}

TYPED_TEST (TestORB_I3, Performance)
{
	test_performance (TypeParam::incarnate ());
}

TYPED_TEST (TestORB_I3, MultiInherit)
{
	I3_var p = TypeParam::incarnate ();

	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);
	EXPECT_EQ (p->op3 (1), 3 * MAGIC_CONST + 1);

	{
		I1_ptr p1 = p;
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p1);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
	}

	{
		I2_ptr p2 = p;
		EXPECT_EQ (p2->op2 (1), 2 * MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p2);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
	}

	{
		Object_ptr obj = p;
		ASSERT_FALSE (is_nil (obj));
		I1_ptr p1 = I1::_narrow (obj);
		ASSERT_FALSE (is_nil (p1));
		I2_ptr p2 = I2::_narrow (obj);
		ASSERT_FALSE (is_nil (p2));
		I3_ptr p3 = I3::_narrow (obj);
		ASSERT_FALSE (is_nil (p3));
		release (p1);
		release (p2);
		release (p3);
	}

	// release (p) must be called automatically.
}

}
