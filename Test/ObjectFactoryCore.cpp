#include "ObjectFactoryCore.h"

namespace CORBA {
namespace Nirvana {

Bridge <ObjectFactory>* const ObjectFactory::singleton_ = STATIC_BRIDGE (ObjectFactoryCore, ObjectFactory);

}
}
