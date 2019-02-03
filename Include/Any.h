#ifndef NIRVANA_ORB_ANY_H_
#define NIRVANA_ORB_ANY_H_

#include "Fixed.h"

namespace CORBA {

class Any
{
public:
	// special helper types needed for boolean, octet, char,
	// and bounded string insertion
	struct from_boolean
	{
		from_boolean (Boolean b) : val (b)
		{}
		Boolean val;
	};
	struct from_octet
	{
		from_octet (Octet o) : val (o)
		{}
		Octet val;
	};
	struct from_char
	{
		from_char (Char c) : val (c)
		{}
		Char val;
	};
	struct from_wchar
	{
		from_wchar (WChar wc) : val (wc)
		{}
		WChar val;
	};
	struct from_string
	{
		from_string (char* s, ULong b,
								 Boolean n = FALSE) :
			val (s), bound (b), nocopy (n)
		{}
		from_string (const char* s, ULong b) :
			val (const_cast<char*>(s)), bound (b),
			nocopy (0)
		{}
		char *val;
		ULong bound;
		Boolean nocopy;
	};
	struct from_wstring
	{
		from_wstring (WChar* s, ULong b,
									Boolean n = FALSE) :
			val (s), bound (b), nocopy (n)
		{}
		from_wstring (const WChar* s, ULong b) :
			val (const_cast<WChar*>(s)), bound (b),
			nocopy (0)
		{}
		WChar *val;
		ULong bound;
		Boolean nocopy;
	};
	struct from_fixed
	{
		from_fixed (const Fixed& f, UShort d, UShort s)
			: val (f), digits (d), scale (s)
		{}
		const Fixed& val;
		UShort digits;
		UShort scale;
	};
	void operator<<=(from_boolean);
	void operator<<=(from_char);
	void operator<<=(from_wchar);
	void operator<<=(from_octet);
	void operator<<=(from_string);
	void operator<<=(from_wstring);
	void operator<<=(from_fixed);

	// special helper types needed for boolean, octet,
	// char, and bounded string extraction
	struct to_boolean
	{
		to_boolean (Boolean &b) : ref (b)
		{}
		Boolean &ref;
	};
	struct to_char
	{
		to_char (Char &c) : ref (c)
		{}
		Char &ref;
	};
	struct to_wchar
	{
		to_wchar (WChar &wc) : ref (wc)
		{}
		WChar &ref;
	};
	struct to_octet
	{
		to_octet (Octet &o) : ref (o)
		{}
		Octet &ref;
	};
	struct to_string
	{
		to_string (const char *&s, ULong b)
			: val (s), bound (b)
		{}
		const char *&val;
		ULong bound;
		// the following constructor is deprecated
		to_string (char *&s, ULong b) : val (s), bound (b)
		{}
	};
	struct to_wstring
	{
		to_wstring (const WChar *&s, ULong b)
			: val (s), bound (b)
		{}
		const WChar *&val;
		ULong bound;
		// the following constructor is deprecated
		to_wstring (WChar *&s, ULong b)
			:val (s), bound (b)
		{}
	};
	struct to_fixed
	{
		to_fixed (Fixed& f, UShort d, UShort s)
			: val (f), digits (d), scale (s)
		{}
		Fixed& val;
		UShort digits;
		UShort scale;
	};
	Boolean operator>>=(to_boolean) const;
	Boolean operator>>=(to_char) const;
	Boolean operator>>=(to_wchar) const;
	Boolean operator>>=(to_octet) const;
	Boolean operator>>=(to_string) const;
	Boolean operator>>=(to_wstring) const;
	Boolean operator>>=(to_fixed) const;

	// other public Any details omitted
private:
	// these functions are private and not implemented
	// hiding these causes compile-time errors for
	// unsigned char
	void operator<<=(unsigned char);
	Boolean operator>>=(unsigned char &) const;
};

}

#endif
