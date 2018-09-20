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

Boolean ObjectBase::_is_a (AbstractBase_ptr base, const char* type_id)
{
	Bridge <Interface>* itf = base->_find_interface (type_id);
	if (itf) {
		(itf->_epv ().release) (itf);
		return TRUE;
	} else
		return FALSE;
}

}
}