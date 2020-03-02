#include <Nirvana/Nirvana.h>
#include "gtest/gtest.h"
#include "Core/Loader.h"

int main (int argc, char** argv)
{
	TestORB::Loader loader;
	testing::InitGoogleTest (&argc, argv);
	return RUN_ALL_TESTS ();
}
