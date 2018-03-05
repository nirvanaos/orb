#include "Object.h"

namespace PortableServer {
namespace Nirvana {

Bridge <ImplementationDef>* ObjectBase::__get_implementation (Bridge <Object>* obj, Environment* env)
{
	return 0;
}

Boolean ObjectBase::__is_equivalent (Bridge <Object>* obj, Bridge <Object>* other_object, Environment* env)
{
	return obj == other_object;
}

}
}