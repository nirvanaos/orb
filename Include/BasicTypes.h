// Nirvana project
// BasicTypes - CORBA basic data types

#ifndef NIRVANA_ORB_BASICTYPES_H_
#define NIRVANA_ORB_BASICTYPES_H_

namespace CORBA {

// Primitive types

typedef bool Boolean;
typedef char Char;
typedef wchar_t WChar;
typedef unsigned char Octet;
typedef short Short;
typedef unsigned short UShort;
typedef long Long;
typedef __int64 LongLong;
typedef unsigned long ULong;
typedef unsigned __int64 ULongLong;
typedef float Float;
typedef double Double;
typedef long double LongDouble;
typedef Boolean& Boolean_out;
typedef Char& Char_out;
typedef WChar& WChar_out;
typedef Octet& Octet_out;
typedef Short& Short_out;
typedef UShort& UShort_out;
typedef Long& Long_out;
typedef LongLong& LongLong_out;
typedef ULong& ULong_out;
typedef ULongLong& ULongLong_out;
typedef Float& Float_out;
typedef Double& Double_out;
typedef LongDouble& LongDouble_out;

typedef Long Flags;
  
#ifndef FALSE
#define FALSE false
#endif
  
#ifndef TRUE
#define TRUE true
#endif
    
} // namespace CORBA

#endif
