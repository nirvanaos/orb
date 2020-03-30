#ifndef NIRVANA_ORB_MARSHALTRAITS_ANY_H_
#define NIRVANA_ORB_MARSHALTRAITS_ANY_H_

#include "MarshalTraits_forward.h"
#include "../Any.h"
#include "Marshal.h"
#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <>
struct MarshalTraits <Any>
{
	static const bool has_marshal = true;

	typedef ABI <Any> ABI;

	static void marshal_in (const Any& src, Marshal_ptr marshaler, ABI& dst);
	static void marshal_out (Any& src, Marshal_ptr marshaler, ABI& dst);
	static void unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Any& dst);
};

}
}

#endif
