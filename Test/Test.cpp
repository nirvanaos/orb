#include "Test_I1.h"
#include <gtest/gtest.h>

using namespace std;
using namespace CORBA;
using namespace Test;

class Instance
{
public:
	static int count ()
	{
		return sm_count;
	}

protected:
	Instance ()
	{
		++sm_count;
	}

	~Instance ()
	{
		--sm_count;
	}

private:
	static int sm_count;
};

int Instance::sm_count = 0;

const Long MAGIC_CONST = 1963;

// Dynamic implementation

class Dynamic :
	public ::PortableServer::Nirvana::Servant <Dynamic, ::Test::I1>,
	public Instance
{
public:
	Dynamic (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new Dynamic (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

// Portable implementation

class Portable :
	public POA_Test::I1,
	public Instance
{
public:
	Portable (Long addendum) :
		m_addendum (addendum)
	{}

	virtual Long op1 (Long p1)
	{
		return p1 + m_addendum;
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new Portable (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

// Static implementation

class Static :
	public ::PortableServer::Nirvana::ServantStatic <Static, ::Test::I1>,
	public Instance
{
public:
	static Long op1 (Long p1)
	{
		return p1 + MAGIC_CONST;
	}

	static ::Test::I1_ptr incarnate ()
	{
		return _this ();
	}
};

// Tied implementation

class Tied :
	public Instance
{
public:
	Tied (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new POA_Test::I1_tie <Tied> (new Tied (MAGIC_CONST)))->_this ();
	}

private:
	Long m_addendum;
};

class TiedDerived :
	public POA_Test::I1_tie <TiedDerived>,
	public Instance
{
public:
	TiedDerived (Long addendum) :
		POA_Test::I1_tie <TiedDerived> (*this),
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new TiedDerived (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

typedef ::testing::Types <Dynamic, Portable, Static, Tied, TiedDerived> ServantTypes;

void test_interface (I1_ptr p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	ASSERT_EQ (p->op1 (1), MAGIC_CONST + 1);
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
		ASSERT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		release (p1);
	}
	ASSERT_FALSE (p->_non_existent ());
	ASSERT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	ASSERT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));
	release (p);
}

void test_performance (I1_ptr p)
{
	for (int i = 0; i < 1000000; ++i)
		p->op1 (2);
	release (p);
}

namespace unittests {

// The fixture for testing BPMXConverterBase functions.
template <class Servant>
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
		ASSERT_EQ (Instance::count (), 0);
	}
};

TYPED_TEST_CASE (TestORB, ServantTypes);

TYPED_TEST (TestORB, Inteface)
{
	test_interface (TypeParam::incarnate ());
}

TYPED_TEST (TestORB, Performance)
{
	test_performance (TypeParam::incarnate ());
}

}
