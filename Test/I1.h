#ifndef NIRVANA_TESTORB_I1_H_
#define NIRVANA_TESTORB_I1_H_

#include "Test_I1_s.h"
#include "TestORB.h"

namespace Test {

class ImplI1
{
public:
	static I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj);
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
};

// Dynamic implementation

class DynamicI1 :
	public CORBA::Nirvana::Servant <DynamicI1, I1>,
	public TestORB::Instance,
	public ImplI1
{
public:
	DynamicI1 (CORBA::Long addendum) :
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

	static I1_ptr incarnate ()
	{
		return PortableServer::Servant_var <DynamicI1> (new DynamicI1 (TestORB::MAGIC_CONST))->_this ();
	}

private:
	CORBA::Long addendum_;
};

// Portable implementation
#ifndef TEST_NO_POA

class PortableI1 :
	public POA_Test::I1,
	public TestORB::Instance,
	public ImplI1
{
public:
	PortableI1 (CORBA::Long addendum) :
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

	static I1_ptr incarnate ()
	{
		return PortableServer::Servant_var <PortableI1> (new PortableI1 (TestORB::MAGIC_CONST))->_this ();
	}

private:
	CORBA::Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

class StaticI1 :
	public ::CORBA::Nirvana::ServantStatic <StaticI1, I1>,
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

	static I1_ptr incarnate ()
	{
		return _this ();
	}

	static const char constant_name[];
};

#endif

// Tied implementation
#ifndef TEST_NO_TIED

class TiedI1 :
	public TestORB::Instance,
	public ImplI1
{
public:
	TiedI1 (CORBA::Long addendum) :
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

	static I1_ptr incarnate ()
	{
		return PortableServer::Servant_var <POA_Test::I1_tie <TiedI1> > (new POA_Test::I1_tie <TiedI1> (new TiedI1 (TestORB::MAGIC_CONST)))->_this ();
	}

private:
	CORBA::Long addendum_;
};

class TiedDerivedI1 :
	public POA_Test::I1_tie <TiedI1>,
	public TiedI1
{
public:
	TiedDerivedI1 (CORBA::Long addendum) :
		POA_Test::I1_tie <TiedI1> (static_cast <TiedI1&> (*this)),
		TiedI1 (addendum)
	{}

	static I1_ptr incarnate ()
	{
		return PortableServer::Servant_var <TiedDerivedI1> (new TiedDerivedI1 (TestORB::MAGIC_CONST))->_this ();
	}
};

#endif

}

#endif
