// Nirvana project
// BasicTypes - CORBA basic data types

#ifndef NIRVANA_ORB_BASICTYPES_H_
#define NIRVANA_ORB_BASICTYPES_H_

#include <stdint.h>
#include "TypeScalar.h"

#define DECLARE_BASIC_TYPE(t) namespace Nirvana { template <> struct Nirvana::Type <t> : Nirvana::TypeScalar <t> {}; }\
typedef Nirvana::Type <t>::C_out t##_out; typedef Nirvana::Type <t>::C_inout t##_inout;

namespace CORBA {

// Primitive types

/// We can not use `bool' built-in type because it is compiler-specific, 
/// but we have to achieve the binary compatibility.
typedef char Boolean;
//DECLARE_BASIC_TYPE (Boolean) // Same as Char

typedef char Char;
DECLARE_BASIC_TYPE (Char)

typedef wchar_t WChar;
DECLARE_BASIC_TYPE (WChar)

typedef uint8_t Octet;
DECLARE_BASIC_TYPE (Octet)

typedef int16_t Short;
DECLARE_BASIC_TYPE (Short)

typedef uint16_t UShort;
DECLARE_BASIC_TYPE (UShort)

typedef int32_t Long;
DECLARE_BASIC_TYPE (Long)

typedef int64_t LongLong;
DECLARE_BASIC_TYPE (LongLong)

typedef uint32_t ULong;
DECLARE_BASIC_TYPE (ULong)

typedef uint64_t ULongLong;
DECLARE_BASIC_TYPE (ULongLong)

typedef float Float;
DECLARE_BASIC_TYPE (Float)

typedef double Double;
DECLARE_BASIC_TYPE (Double)

typedef long double LongDouble;
DECLARE_BASIC_TYPE (LongDouble)

typedef Long Flags;
  
#ifndef FALSE
#define FALSE 0
#endif
  
#ifndef TRUE
#define TRUE 1
#endif

} // namespace CORBA

#undef DECLARE_BASIC_TYPE

#endif
