#ifndef NIRVANA_ORB_TC_TYPECODE_H_
#define NIRVANA_ORB_TC_TYPECODE_H_

#include "TypeCodeImpl.h"
#include "../TypeCode.h"

namespace CORBA {
namespace Nirvana {

class TC_TypeCode :
	public TypeCodeWithId <TC_TypeCode, tk_TypeCode, TypeCode::interface_id_>,
	public TypeCodeOps <TypeCode_var>
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return "TypeCode";
	}
};

}
}

#endif
