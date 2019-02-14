#include "ObjectAdapterCore.h"

namespace CORBA {
namespace Nirvana {

ObjectAdapter_ptr g_system = InterfaceStatic <ObjectAdapterCore, ObjectAdapter>::_bridge ();

}
}
