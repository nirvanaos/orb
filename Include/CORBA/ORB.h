#ifndef NIRVANA_ORB_ORB_H_
#define NIRVANA_ORB_ORB_H_

#include "CORBA_Environment.h"

namespace CORBA {

class ORB
{
public:
	static void create_environment (Environment_out);
};

}

#endif
