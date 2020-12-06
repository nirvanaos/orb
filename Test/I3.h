#ifndef NIRVANA_TESTORB_I3_H_
#define NIRVANA_TESTORB_I3_H_

#include "Test_I3_s.h"
#include "TestORB.h"
#include "I1.h"

namespace Test {

// Dynamic implementation

class DynamicI3 :
	public CORBA::Nirvana::Servant <DynamicI3, I3>,
	public TestORB::Instance,
	public ImplI1
{
public:
	DynamicI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		return p1 + addendum_;
	}

	static void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	CORBA::Long op2 (CORBA::Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	CORBA::Long op3 (CORBA::Long p1) const
	{
		return p1 + 3 * addendum_;
	}

	static I3_ptr incarnate ()
	{
		return PortableServer::Servant_var <DynamicI3> (new DynamicI3 (TestORB::MAGIC_CONST))->_this ();
	}

private:
	CORBA::Long addendum_;
};

// Portable implementation

#ifndef TEST_NO_POA

class PortableI3 :
	public POA_Test::I3,
	public TestORB::Instance,
	public ImplI1
{
public:
	PortableI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	virtual CORBA::Long op1 (CORBA::Long p1)
	{
		return p1 + addendum_;
	}

	virtual void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	virtual I1_ptr object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj)
	{
		return ImplI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
	{
		return ImplI1::string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s)
	{
		return ImplI1::seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Any any_op (const CORBA::Any& in_a, CORBA::Any& out_a, CORBA::Any& inout_a)
	{
		return ImplI1::any_op (in_a, out_a, inout_a);
	}

	virtual CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * addendum_;
	}

	virtual CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * addendum_;
	}

	static I3_ptr incarnate ()
	{
		return PortableServer::Servant_var <PortableI3> (new PortableI3 (TestORB::MAGIC_CONST))->_this ();
	}

private:
	CORBA::Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

class StaticI3 :
	public CORBA::Nirvana::ServantStatic <StaticI3, I3>,
	public TestORB::Instance,
	public ImplI1
{
public:
	static CORBA::Long op1 (CORBA::Long p1)
	{
		return p1 + TestORB::MAGIC_CONST;
	}

	static void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	static CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * TestORB::MAGIC_CONST;
	}

	static CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * TestORB::MAGIC_CONST;
	}

	static I3_ptr incarnate ()
	{
		return _this ();
	}

	static const char constant_name[];
};

#endif

// Tied implementation
#ifndef TEST_NO_TIED

class TiedI3 :
	public TestORB::Instance,
	public ImplI1
{
public:
	TiedI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		return p1 + addendum_;
	}

	void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	CORBA::Long op2 (CORBA::Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	CORBA::Long op3 (CORBA::Long p1) const
	{
		return p1 + 3 * addendum_;
	}

	static I3_ptr incarnate ()
	{
		return PortableServer::Servant_var <POA_Test::I3_tie <TiedI3> > (new POA_Test::I3_tie <TiedI3> (new TiedI3 (TestORB::MAGIC_CONST)))->_this ();
	}

private:
	CORBA::Long addendum_;
};

class TiedDerivedI3 :
	public POA_Test::I3_tie <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (CORBA::Long addendum) :
		POA_Test::I3_tie <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}

	static I3_ptr incarnate ()
	{
		return PortableServer::Servant_var <TiedDerivedI3> (new TiedDerivedI3 (TestORB::MAGIC_CONST))->_this ();
	}
};

#endif

}

#endif
