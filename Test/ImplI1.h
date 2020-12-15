#ifndef NIRVANA_TESTORB_IMPLI1_H_
#define NIRVANA_TESTORB_IMPLI1_H_

#include "Test_I1.h"

namespace Test {

class ImplI1
{
public:
	static I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj);
	static std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	static std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	static CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
};

}

#endif
