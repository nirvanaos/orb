#ifndef NIRVANA_ORB_TYPEBASIC_H_
#define NIRVANA_ORB_TYPEBASIC_H_

#include "TypeScalar.h"
#include "BasicTypes.h"

namespace CORBA {
namespace Nirvana {

#define BASIC_TYPE(t) template <> struct Type <t> : TypeScalar <t> {}; typedef Type <t>::C_out t##_out; typedef Type <t>::C_inout t##_inout;

//BASIC_TYPE (Boolean) // Same as Char
BASIC_TYPE (Char)
BASIC_TYPE (WChar)
BASIC_TYPE (Octet)
BASIC_TYPE (Short)
BASIC_TYPE (UShort)
BASIC_TYPE (Long)
BASIC_TYPE (LongLong)
BASIC_TYPE (ULong)
BASIC_TYPE (ULongLong)
BASIC_TYPE (Float)
BASIC_TYPE (Double)
BASIC_TYPE (LongDouble)

#undef BASIC_TYPE
}
}

#endif
