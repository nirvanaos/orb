#ifndef NIRVANA_TESTORB_TEST_H_
#define NIRVANA_TESTORB_TEST_H_

#include <gtest/gtest.h>
#include <Mock/TestMock.h>
#include "Mock/MockBinder.h"

namespace TestORB {

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
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		Base::TearDown ();
	}

private:
	::Nirvana::Test::MockBinder binder_;
};

}

#endif


