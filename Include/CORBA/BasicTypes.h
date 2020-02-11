// Nirvana project
// BasicTypes - CORBA basic data types

#ifndef NIRVANA_ORB_BASICTYPES_H_
#define NIRVANA_ORB_BASICTYPES_H_

#include <stdint.h>

namespace CORBA {

// Primitive types

/// We can not use `bool' built-in type because it is compiler-specific, but we have to achieve the binary compatibility.
typedef char Boolean;

typedef char Char;
typedef wchar_t WChar;
typedef uint8_t Octet;
typedef int16_t Short;
typedef uint16_t UShort;
typedef int32_t Long;
typedef int64_t LongLong;
typedef uint32_t ULong;
typedef uint64_t ULongLong;
typedef float Float;
typedef double Double;
typedef long double LongDouble;

typedef Long Flags;
  
#ifndef FALSE
#define FALSE 0
#endif
  
#ifndef TRUE
#define TRUE 1
#endif

typedef const char* RepositoryId;
typedef const char* Identifier;

} // namespace CORBA

#endif
