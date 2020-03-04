#ifndef NIRVANA_TESTORB_TEST_H_
#define NIRVANA_TESTORB_TEST_H_

#include <gtest/gtest.h>
#include <Mock/MockMemory.h>
#include "RootPOA.h"

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
		Instance::count_ = 0;
		allocated_ = ::Nirvana::Test::allocated_bytes ();
		RootPOA::create ();
	}

	virtual void TearDown ()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
		RootPOA::destroy ();
		ASSERT_EQ (Instance::count (), 0);
		if (!HasFatalFailure ())
			EXPECT_EQ (::Nirvana::Test::allocated_bytes (), allocated_);
	}

private:
	size_t allocated_;
};

}

#endif


