#ifndef NIRVANA_TESTORB_I3_H_
#define NIRVANA_TESTORB_I3_H_

#include "Test_I3_s.h"
#include "Test.h"

namespace TestORB {

using namespace CORBA;
using namespace Test;

// Dynamic implementation

class DynamicI3 :
	public ::CORBA::Nirvana::Servant <DynamicI3, ::Test::I3>,
	public Instance
{
public:
	DynamicI3 (Long addendum) :
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

	Long op2 (Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	Long op3 (Long p1) const
	{
		return p1 + 3 * addendum_;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new DynamicI3 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

// Portable implementation

#ifndef TEST_NO_POA

class PortableI3 :
	public virtual POA_Test::I3,
	public Instance
{
public:
	PortableI3 (Long addendum) :
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

	virtual Long op2 (Long p1)
	{
		return p1 + 2 * addendum_;
	}

	virtual Long op3 (Long p1)
	{
		return p1 + 3 * addendum_;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new PortableI3 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

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

#endif

// Tied implementation
#ifndef TEST_NO_TIED

class TiedI3 :
	public Instance
{
public:
	TiedI3 (Long addendum) :
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

	Long op2 (Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	Long op3 (Long p1) const
	{
		return p1 + 3 * addendum_;
	}

	static ::Test::I3_ptr incarnate ()
	{
		return (new POA_Test::I3_tie <TiedI3> (new TiedI3 (MAGIC_CONST)))->_this ();
	}

private:
	Long addendum_;
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
	Long addendum_;
};

#endif

}

#endif
