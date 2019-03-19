#include "ObjectFactoryCore.h"

namespace CORBA {
namespace Nirvana {

ObjectFactory_ptr g_object_factory = InterfaceStatic <ObjectFactoryCore, ObjectFactory>::_bridge ();

}
}
