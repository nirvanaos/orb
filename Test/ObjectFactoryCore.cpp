#include "ObjectAdapterCore.h"

namespace CORBA {
namespace Nirvana {

ObjectAdapter_ptr g_object_adapter = InterfaceStatic <ObjectAdapterCore, ObjectAdapter>::_bridge ();

}
}
