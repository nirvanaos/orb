#ifndef NIRVANA_ORB_MARSHALTRAITS_ANY_H_
#define NIRVANA_ORB_MARSHALTRAITS_ANY_H_

#include "MarshalTraits_forward.h"
#include "../Any.h"
#include "PlatformMarshal.h"
#include "PlatformUnmarshal.h"

namespace CORBA {
namespace Nirvana {

template <>
struct MarshalTraits <Any>
{
	typedef ABI <Any> ABI;

	static void marshal_in (const Any& src, PlatformMarshal_ptr marshaler, ABI& dst);
	static void marshal_out (Any& src, PlatformMarshal_ptr marshaler, ABI& dst);
	static void unmarshal (ABI& src, PlatformUnmarshal_ptr unmarshaler, Any& dst);
};

}
}

#endif
