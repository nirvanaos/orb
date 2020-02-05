#ifndef NIRVANA_ORB_TYPECODEOBJECT_H_
#define NIRVANA_ORB_TYPECODEOBJECT_H_

#include "TypeCodeImpl.h"
#include "Object_c.h"

namespace CORBA {
namespace Nirvana {


class TC_Object :
	public TypeCodeWithId <TC_Object, tk_objref, Object::interface_id_>,
	public TypeCodeOps <Object_var>
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return "Object";
	}
};

}
}

#endif
