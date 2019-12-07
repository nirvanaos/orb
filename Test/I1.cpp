#include "I1.h"

using namespace Test;

namespace TestORB {

I1_ptr ImplI1::object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj)
{
	I1_var tmp (std::move (inout_obj));
	out_obj = I1::_duplicate (in_obj);
	inout_obj = I1::_duplicate (in_obj);
	return tmp._retn ();
}

std::string ImplI1::string_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
{
	std::string tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

std::string ImplI1::bstring_op (const std::string& in_s, std::string& out_s, std::string& inout_s)
{
	std::string tmp (std::move (inout_s));
	out_s = in_s;
	inout_s = in_s;
	return tmp;
}

}