#ifndef NIRVANA_TESTORB_I3_H_
#define NIRVANA_TESTORB_I3_H_

#include "Test_I3_s.h"
#include "TestORB.h"
#include "I1.h"

namespace TestORB {

using namespace CORBA;
using namespace Test;

// Dynamic implementation

class DynamicI3 :
	public ::CORBA::Nirvana::Servant <DynamicI3, ::Test::I3>,
	public Instance,
	public ImplI1
{
public:
	DynamicI3 (Long addendum) :
		addendum_ (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + addendum_;
	}

	static void throw_no_implement ()
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
		return PortableServer::Servant_var <DynamicI3> (new DynamicI3 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

// Portable implementation

#ifndef TEST_NO_POA

class PortableI3 :
	public POA_Test::I3,
	public Instance,
	public ImplI1
{
public:
	PortableI3 (Long addendum) :
		addendum_ (addendum)
	{}

	virtual Long op1 (Long p1)
	{
		return p1 + addendum_;
	}

	virtual void throw_no_implement ()
	{
		throw NO_IMPLEMENT ();
	}

	virtual ::Test::I1_ptr object_op (::Test::I1_ptr in_obj, ::Test::I1_var& out_obj, ::Test::I1_var& inout_obj)
	{
		return ImplI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
	{
		return ImplI1::string_op (in_s, out_s, inout_s);
	}

	virtual std::vector <Long> seq_op (const std::vector <Long>& in_s, std::vector <Long>& out_s, std::vector <Long>& inout_s)
	{
		return ImplI1::seq_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Any any_op (const CORBA::Any& in_a, CORBA::Any& out_a, CORBA::Any& inout_a)
	{
		return ImplI1::any_op (in_a, out_a, inout_a);
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
		return PortableServer::Servant_var <PortableI3> (new PortableI3 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

class StaticI3 :
	public ::CORBA::Nirvana::ServantStatic <StaticI3, ::Test::I3>,
	public Instance,
	public ImplI1
{
public:
	static Long op1 (Long p1)
	{
		return p1 + MAGIC_CONST;
	}

	static void throw_no_implement ()
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

	static const char constant_name[];
};

#endif

// Tied implementation
#ifndef TEST_NO_TIED

class TiedI3 :
	public Instance,
	public ImplI1
{
public:
	TiedI3 (Long addendum) :
		addendum_ (addendum)
	{}

	Long op1 (Long p1) const
	{
		return p1 + addendum_;
	}

	void throw_no_implement ()
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
		return PortableServer::Servant_var <POA_Test::I3_tie <TiedI3> > (new POA_Test::I3_tie <TiedI3> (new TiedI3 (MAGIC_CONST)))->_this ();
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
		POA_Test::I3_tie <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}

	static ::Test::I3_ptr incarnate ()
	{
		return PortableServer::Servant_var <TiedDerivedI3> (new TiedDerivedI3 (MAGIC_CONST))->_this ();
	}
};

#endif

}

#endif
