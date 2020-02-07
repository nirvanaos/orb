#ifndef NIRVANA_ORB_ANY_H_
#define NIRVANA_ORB_ANY_H_

#include "AnyABI.h"
#include <Nirvana/basic_string.h>

//#include "Fixed.h"

namespace CORBA {

class Any :
	public Nirvana::AnyABI
{
public:
	Any ()
	{
		reset ();
	}

	Any (const Any& src)
	{
		reset ();
		copy_from (src.type (), src.data ());
	}

	Any (Any&& src) NIRVANA_NOEXCEPT :
		AnyABI (std::move (src))
	{}

	~Any ()
	{
		clear ();
	}

	Any& operator = (const Any& src)
	{
		copy_from (src.type (), src.data ());
		return *this;
	}

	Any& operator = (Any&& src) NIRVANA_NOEXCEPT
	{
		clear ();
		data_ = src.data_;
		src.reset ();
		return *this;
	}

	TypeCode_ptr type () const
	{
		return Nirvana::AnyABI::type ();
	}

	void type (TypeCode_ptr alias);

	const void* data () const
	{
		return is_large () ? large_pointer () : small_pointer ();
	}

	void clear ();

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
	/*
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
		from_wstring (WChar* s, ULong b, Boolean n = FALSE) :
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
	*/
	/*
	struct from_fixed
	{
		from_fixed (const Fixed& f, UShort d, UShort s)
			: val (f), digits (d), scale (s)
		{}
		const Fixed& val;
		UShort digits;
		UShort scale;
	};*/
	void operator <<= (from_boolean);
	void operator <<= (from_char);
	void operator <<= (from_wchar);
	void operator <<= (from_octet);
	//void operator <<= (from_string);
	//void operator <<= (from_wstring);
	//void operator <<= (from_fixed);

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
	/*
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
	*/
	/*
	struct to_fixed
	{
		to_fixed (Fixed& f, UShort d, UShort s)
			: val (f), digits (d), scale (s)
		{}
		Fixed& val;
		UShort digits;
		UShort scale;
	};*/

	Boolean operator >>= (to_boolean) const;
	Boolean operator >>= (to_char) const;
	Boolean operator >>= (to_wchar) const;
	Boolean operator >>= (to_octet) const;
	//Boolean operator >>= (to_string) const;
	//Boolean operator >>= (to_wstring) const;
	//Boolean operator >>= (to_fixed) const;

	void copy_from (TypeCode_ptr tc, const void* val);
	void move_from (TypeCode_ptr tc, void* val);

private:
	void* prepare (TypeCode_ptr tc);

	void set_type (TypeCode_ptr tc)
	{
		AnyABI::type (TypeCode::_duplicate (tc));
	}

	// these functions are private and not implemented
	// hiding these causes compile-time errors for
	// unsigned char
	void operator <<= (unsigned char);
	Boolean operator >>= (unsigned char &) const;
};

void operator <<= (Any&, Short);
void operator <<= (Any&, UShort);
void operator <<= (Any&, Long);
void operator <<= (Any&, ULong);
void operator <<= (Any&, Float);
void operator <<= (Any&, Double);
void operator <<= (Any&, LongLong);
void operator <<= (Any&, ULongLong);
void operator <<= (Any&, LongDouble);
void operator <<= (Any&, const std::string&);
void operator <<= (Any&, std::string&&);
void operator <<= (Any&, const std::wstring&);
void operator <<= (Any&, std::wstring&&);
void operator <<= (Any&, const Any&);
void operator <<= (Any&, Any&&);
void operator <<= (Any&, const Exception&);

Boolean operator >>= (const Any&, Short&);
Boolean operator >>= (const Any&, UShort&);
Boolean operator >>= (const Any&, Long&);
Boolean operator >>= (const Any&, ULong&);
Boolean operator >>= (const Any&, Float&);
Boolean operator >>= (const Any&, Double&);
Boolean operator >>= (const Any&, LongLong&);
Boolean operator >>= (const Any&, ULongLong&);
Boolean operator >>= (const Any&, LongDouble&);
Boolean operator >>= (const Any&, const std::string*&);
Boolean operator >>= (const Any&, const std::wstring*&);
Boolean operator >>= (const Any&, const Any*&);
Boolean operator >>= (const Any&, const SystemException*&);

}

#endif
