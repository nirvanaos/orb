#include <CORBA/CORBA.h>
#include <gtest/gtest.h>
#include <Mock/TestMock.h>
#include <functional>

using namespace CORBA;

/*
void must_not_compile1 (const Test::I3_var& var)
{
	Test::I1_var i1 = var; // Implicit cast between var must cause a compilation error
}

bool must_not_compile2 (const Test::I3_ptr p1, const Test::I3_ptr p2)
{
	return p1 == p2;
}

bool must_not_compile3 (const Test::I3_var p1, const Test::I3_var p2)
{
	return p1 == p2;
}
*/
namespace TestORB {

class TestORB : public Nirvana::Test::TestMock
{
public:
};

TEST_F (TestORB, RepositoryId)
{
	EXPECT_TRUE (CORBA::Internal::RepId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.0"));
	EXPECT_TRUE (CORBA::Internal::RepId::compatible ("IDL:aaa/bbb/type:1.1", "IDL:aaa/bbb/type:1.0"));
	EXPECT_FALSE (CORBA::Internal::RepId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.1"));
	EXPECT_FALSE (CORBA::Internal::RepId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/other:1.0"));
	EXPECT_FALSE (CORBA::Internal::RepId::compatible ("IDL:aaa/bbb/type:1.0", "aaa/bbb/type:1.0"));

	const char rep_id1 [] = "IDL:Test/I1:1.0";
	const char rep_id3 [] = "IDL:Test/I3:1.0";

	EXPECT_LT (CORBA::Internal::RepId::compare (rep_id1, std::size (rep_id1) - 1, rep_id3, std::size (rep_id3) - 1), 0);
}

TEST_F (TestORB, Environment)
{
	CORBA::Internal::Environment ne;
	CORBA::NO_MEMORY no_mem;
	set_exception (&ne, no_mem);
	const CORBA::Exception* ex = ne.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	CORBA::Internal::Environment ne1 (std::move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
}

size_t string_size (CORBA::Internal::String_in string_in)
{
	return static_cast <const std::string&> (string_in).size ();
}

TEST_F (TestORB, String_in)
{
	EXPECT_EQ (string_size ("test"), 4u);
	{
		std::string test = "test";
		EXPECT_EQ (string_size (test), test.size ());
	}
	{
		const char test [] = "test";
		EXPECT_EQ (string_size (test), 4u);
	}
	{
		char test [32] = "test";
		EXPECT_EQ (string_size (test), 4u);
	}
	{
		const char test [32] = "test"; // Strange case
		EXPECT_EQ (string_size (test), 31u);
	}
}

TEST_F (TestORB, Array)
{
	using namespace CORBA::Internal;

	typedef std::array <std::array <int, 3>, 4> Arr;
	EXPECT_EQ (ArrayTraits <Arr>::size, 3u * 4u);

	EXPECT_EQ (Type <Arr>::total_size, 3u * 4u);
	EXPECT_TRUE (Type <Arr>::is_CDR);
}

}
