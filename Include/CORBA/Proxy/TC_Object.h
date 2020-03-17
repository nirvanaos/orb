#ifndef NIRVANA_ORB_TC_OBJECT_H_
#define NIRVANA_ORB_TC_OBJECT_H_

#include "TypeCodeImpl.h"
#include "../Object.h"

namespace CORBA {
namespace Nirvana {

class TC_Object :
	public TypeCodeStatic <TC_Object, TypeCodeWithId <tk_objref, Object::interface_id_>, TypeCodeOps <Object_var> >
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
