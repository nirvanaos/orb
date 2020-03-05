#ifndef NIRVANA_ORB_MARSHALTRAITS_STRING_H_
#define NIRVANA_ORB_MARSHALTRAITS_STRING_H_

#include "MarshalTraits_forward.h"
#include "String.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
struct MarshalTraits <StringT <C> >
{
};

}
}

#endif
