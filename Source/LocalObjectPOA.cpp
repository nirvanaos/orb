#include <CORBA/LocalObjectPOA.h>

namespace CORBA {
namespace Nirvana {

Interface* ServantPOA <Object>::__get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__get_interface (obj, env);
}

ABI_boolean ServantPOA <Object>::__is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__is_a (obj, type_id, env);
}

ABI_boolean ServantPOA <Object>::__non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__non_existent (obj, env);
}

}
}
