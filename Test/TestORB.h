#ifndef NIRVANA_TESTORB_TEST_H_
#define NIRVANA_TESTORB_TEST_H_

#include <gtest/gtest.h>
#include <Mock/TestMock.h>
#include "Mock/MockPOA.h"
#include "Mock/MockBinder.h"

namespace TestORB {

class Instance
{
public:
	static int count ()
	{
		return count_;
	}

protected:
	Instance ()
	{
		++count_;
	}

	~Instance ()
	{
		--count_;
	}

public:
	static int count_;
};

const long MAGIC_CONST = 1963;

class TestORB :
	public ::Nirvana::Test::TestMock
{
	typedef ::Nirvana::Test::TestMock Base;
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
		Base::SetUp ();
		Instance::count_ = 0;
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		EXPECT_EQ (Instance::count (), 0);
		Base::TearDown ();
	}

private:
	::Nirvana::Test::MockBinder binder_;
	::Nirvana::Test::MockPOA poa_;
};

}

#endif


