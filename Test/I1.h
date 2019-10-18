#ifndef NIRVANA_TESTORB_I1_H_
#define NIRVANA_TESTORB_I1_H_

#include "Test_I1_s.h"
#include "TestORB.h"

namespace TestORB {

using namespace CORBA;
using namespace Test;

class ImplI1
{
public:
	static I1_ptr object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj);
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static CORBA::Nirvana::String_var <char, 20> bstring_op (const std::string& in_s, 
		CORBA::Nirvana::String_out <char, 20> out_s, CORBA::Nirvana::String_inout <char, 20> inout_s);
};

// Dynamic implementation

class DynamicI1 :
	public ::CORBA::Nirvana::Servant <DynamicI1, ::Test::I1>,
	public Instance,
	public ImplI1
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
		return PortableServer::Servant_var <DynamicI1> (new DynamicI1 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

// Portable implementation
#ifndef TEST_NO_POA

class PortableI1 :
	public virtual POA_Test::I1,
	public Instance,
	public ImplI1
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

	virtual ::Test::I1_ptr object_op (::Test::I1_ptr in_obj, ::Test::I1_var& out_obj, ::Test::I1_var& inout_obj)
	{
		return ImplI1::object_op (in_obj, out_obj, inout_obj);
	}

	virtual std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
	{
		return ImplI1::string_op (in_s, out_s, inout_s);
	}

	virtual CORBA::Nirvana::String_var <char, 20> bstring_op (const std::string& in_s,
		CORBA::Nirvana::String_out <char, 20> out_s, CORBA::Nirvana::String_inout <char, 20> inout_s)
	{
		return ImplI1::bstring_op (in_s, out_s, inout_s);
	}

	static ::Test::I1_ptr incarnate ()
	{
		return PortableServer::Servant_var <PortableI1> (new PortableI1 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

#endif

// Static implementation
#ifndef TEST_NO_STATIC

class StaticI1 :
	public ::CORBA::Nirvana::ServantStatic <StaticI1, ::Test::I1>,
	public Instance,
	public ImplI1
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
	public Instance,
	public ImplI1
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
		return PortableServer::Servant_var <POA_Test::I1_tie <TiedI1> > (new POA_Test::I1_tie <TiedI1> (new TiedI1 (MAGIC_CONST)))->_this ();
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
		return PortableServer::Servant_var <TiedDerivedI1> (new TiedDerivedI1 (MAGIC_CONST))->_this ();
	}

private:
	Long addendum_;
};

#endif

}

#endif
