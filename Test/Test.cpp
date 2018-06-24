#include "Test_I3.h"
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

class DynamicI1 :
	public ::CORBA::Nirvana::Servant <DynamicI1, ::Test::I1>,
	public Instance
{
public:
	DynamicI1 (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	static void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new DynamicI1 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

class DynamicI3 :
	public ::CORBA::Nirvana::Servant <DynamicI3, ::Test::I3>,
	public Instance
{
public:
	DynamicI3 (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	static void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	Long op2 (Long p1) const
	{
		return p1 + 2 * m_addendum;
	}

	Long op3 (Long p1) const
	{
		return p1 + 3 * m_addendum;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new DynamicI3 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

// Portable implementation

class PortableI1 :
	public POA_Test::I1,
	public Instance
{
public:
	PortableI1 (Long addendum) :
		m_addendum (addendum)
	{}

	virtual Long op1 (Long p1)
	{
		return p1 + m_addendum;
	}

	virtual void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new PortableI1 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

class PortableI3 :
	public POA_Test::I3,
	public Instance
{
public:
	PortableI3 (Long addendum) :
		m_addendum (addendum)
	{}

	virtual Long op1 (Long p1)
	{
		return p1 + m_addendum;
	}

	virtual void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	virtual Long op2 (Long p1)
	{
		return p1 + 2 * m_addendum;
	}

	virtual Long op3 (Long p1)
	{
		return p1 + 3 * m_addendum;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new PortableI3 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

// Static implementation

class StaticI1 :
	public ::CORBA::Nirvana::ServantStatic <StaticI1, ::Test::I1>,
	public Instance
{
public:
	static Long op1 (Long p1)
	{
		return p1 + MAGIC_CONST;
	}

	static void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static ::Test::I1_ptr incarnate ()
	{
		return _this ();
	}
};

class StaticI3 :
	public ::CORBA::Nirvana::ServantStatic <StaticI3, ::Test::I3>,
	public Instance
{
public:
	static Long op1 (Long p1)
	{
		return p1 + MAGIC_CONST;
	}

	static void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static Long op2 (Long p1)
	{
		return p1 + 2 * MAGIC_CONST;
	}

	static Long op3 (Long p1)
	{
		return p1 + 3 * MAGIC_CONST;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return _this ();
	}
};

// Tied implementation

class TiedI1 :
	public Instance
{
public:
	TiedI1 (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static ::Test::I1_ptr incarnate ()
	{
		return (new POA_Test::I1_tie <TiedI1> (new TiedI1 (MAGIC_CONST)))->_this ();
	}

private:
	Long m_addendum;
};

class TiedI3 :
	public Instance
{
public:
	TiedI3 (Long addendum) :
		m_addendum (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + m_addendum;
	}

	void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	Long op2 (Long p1) const
	{
		return p1 + 2 * m_addendum;
	}

	Long op3 (Long p1) const
	{
		return p1 + 3 * m_addendum;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new POA_Test::I3_tie <TiedI3> (new TiedI3 (MAGIC_CONST)))->_this ();
	}

private:
	Long m_addendum;
};

class TiedDerivedI1 :
	public POA_Test::I1_tie <TiedI1>,
	public TiedI1
{
public:
	TiedDerivedI1 (Long addendum) :
		TiedI1 (addendum),
		POA_Test::I1_tie <TiedI1> (static_cast <TiedI1&> (*this))
	{}

	static ::Test::I1_ptr incarnate ()
	{
		return (new TiedDerivedI1 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

class TiedDerivedI3 :
	public POA_Test::I3_tie <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (Long addendum) :
		TiedI3 (addendum),
		POA_Test::I3_tie <TiedI3> (static_cast <TiedI3&> (*this))
	{}

	static ::Test::I3_ptr incarnate ()
	{
		return (new TiedDerivedI3 (MAGIC_CONST))->_this ();
	}

private:
	Long m_addendum;
};

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

namespace unittests {

class Test :
	public ::testing::Test
{
protected:
	Test ()
	{}

	virtual ~Test ()
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

TEST_F (Test, RepositoryId)
{
	EXPECT_TRUE (Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.0"));
	EXPECT_TRUE (Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.1", "IDL:aaa/bbb/type:1.0"));
	EXPECT_FALSE (Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.1"));
	EXPECT_FALSE (Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/other:1.0"));
	EXPECT_FALSE (Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "aaa/bbb/type:1.0"));
}

// The fixture for testing simple interface.

typedef ::testing::Types <DynamicI1, PortableI1, StaticI1, TiedI1, TiedDerivedI1> ServantTypesI1;

template <class Servant>
class TestI1 :
	public ::testing::Test
{
protected:
	TestI1 ()
	{}

	virtual ~TestI1 ()
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

TYPED_TEST_CASE (TestI1, ServantTypesI1);

TYPED_TEST (TestI1, Inteface)
{
	test_interface (TypeParam::incarnate ());
}

TYPED_TEST (TestI1, Performance)
{
	test_performance (TypeParam::incarnate ());
}

TYPED_TEST (TestI1, Exception)
{
	I1_ptr p = TypeParam::incarnate ();
	EXPECT_THROW (p->throw_NO_IMPLEMENT (), NO_IMPLEMENT);
	release (p);
}

// The fixture for testing complex interface.

typedef ::testing::Types <DynamicI3, PortableI3, StaticI3, TiedI3, TiedDerivedI3> ServantTypesI3;

template <class Servant>
class TestI3 :
	public ::testing::Test
{
protected:
	TestI3 ()
	{}

	virtual ~TestI3 ()
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

TYPED_TEST_CASE (TestI3, ServantTypesI3);

TYPED_TEST (TestI3, Inteface)
{
	test_interface (TypeParam::incarnate ());
}

TYPED_TEST (TestI3, Performance)
{
	test_performance (TypeParam::incarnate ());
}

TYPED_TEST (TestI3, MultiInherit)
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
