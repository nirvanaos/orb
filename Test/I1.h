#ifndef NIRVANA_TESTORB_I1_H_
#define NIRVANA_TESTORB_I1_H_

#include "Test_I1_s.h"
#include "Test.h"

namespace POA_Test {

using namespace CORBA;
using namespace Test;
using namespace TestORB;

// Dynamic implementation

class DynamicI1 :
	public ::CORBA::Nirvana::Servant <DynamicI1, ::Test::I1>,
	public Instance
{
public:
	DynamicI1 (Long addendum) :
		addendum_ (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + addendum_;
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
	Long addendum_;
};

// Portable implementation
#ifndef TEST_NO_POA

class PortableI1 :
	public ::POA_Test::I1,
	public Instance
{
public:
	PortableI1 (Long addendum) :
		addendum_ (addendum)
	{}

	virtual Long op1 (Long p1)
	{
		return p1 + addendum_;
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
	Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

class I1_static :
	public ::CORBA::Nirvana::ServantStatic < ::Test::I1>,
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

#endif

// Tied implementation
#ifndef TEST_NO_TIED

class TiedI1 :
	public Instance
{
public:
	TiedI1 (Long addendum) :
		addendum_ (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + addendum_;
	}

	void throw_NO_IMPLEMENT ()
	{
		throw NO_IMPLEMENT ();
	}

	static Test::I1_ptr incarnate ()
	{
		return (new POA_Test::I1_tie <TiedI1> (new TiedI1 (MAGIC_CONST)))->_this ();
	}

private:
	Long addendum_;
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
	Long addendum_;
};

#endif

}

#endif
