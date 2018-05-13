#include "Object.h"

namespace CORBA {
namespace Nirvana {

Bridge <ImplementationDef>* ObjectBase::__get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return 0;
}

Boolean ObjectBase::__is_equivalent (Bridge <Object>* obj, Bridge <Object>* other_object, EnvironmentBridge* env)
{
	return obj == other_object;
}

}
}