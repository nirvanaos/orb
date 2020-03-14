// Nirvana project
// BasicTypes - CORBA basic data types

#ifndef NIRVANA_ORB_BASICTYPES_H_
#define NIRVANA_ORB_BASICTYPES_H_

#include <stdint.h>
#include "Boolean.h"
#include "TypeFixLen.h"

#define DECLARE_BASIC_TYPE(T) namespace Nirvana { template <> struct Nirvana::Type <T> : Nirvana::TypeFixLen <T> {}; }\
typedef Nirvana::Type <T>::C_out T##_out; typedef Nirvana::Type <T>::C_inout T##_inout;

namespace CORBA {

// Primitive types

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
  
} // namespace CORBA

#undef DECLARE_BASIC_TYPE

#endif
