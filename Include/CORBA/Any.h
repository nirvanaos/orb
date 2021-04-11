/// \file Any.h
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_ANY_H_
#define NIRVANA_ORB_ANY_H_

#include "TypeVarLen.h"
#include "ABI_Any.h"
#include "String.h"
//#include "Fixed.h"

namespace CORBA {

class Any : private Nirvana::ABI <Any>
{
	typedef Nirvana::ABI <Any> ABI;
public:
	Any ()
	{
		reset ();
	}

	Any (const Any& src)
	{
		reset ();
		copy_from (src);
	}

	Any (Any&& src) NIRVANA_NOEXCEPT :
		ABI (src)
	{
		src.reset ();
	}

	~Any ()
	{
		clear ();
	}

	Any& operator = (const Any& src)
	{
		if (this != &src)
			copy_from (src);
		return *this;
	}

	Any& operator = (Any&& src) NIRVANA_NOEXCEPT
	{
		if (this != &src) {
			clear ();
			ABI::operator = (src);
			src.reset ();
		}
		return *this;
	}

	TypeCode_ptr type () const
	{
		return ABI::type ();
	}

	void type (TypeCode_ptr alias);

	const void* data () const;

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
		from_string (char* s, ULong b, Boolean n = FALSE) :
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
	friend struct Nirvana::Type <Any>;

	void copy_from (const Any& src);
	void* prepare (TypeCode_ptr tc);
	void set_type (TypeCode_ptr tc);
	void set_type (TypeCode_var&& tc);

	// these functions are private and not implemented
	// hiding these causes compile-time errors for
	// unsigned char
	void operator <<= (unsigned char);
	Boolean operator >>= (unsigned char &) const;
};

namespace Nirvana {

template <>
struct Type <Any> : public TypeVarLen <Any, true>
{
	typedef TypeVarLen <Any, true> Base;
	typedef ABI <Any> ABI_type;

	static void check (const ABI_type& any);

	class C_out : public Base::C_out
	{
	public:
		C_out (Any& any) :
			Base::C_out (any)
		{
			any.clear ();
		}
	};

	static Any& out (typename Base::ABI_out p)
	{
		Any& val = Base::out (p);
		// Must be empty
		if (val.type ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static TypeCode_ptr type_code ();

	static void marshal_in (const Any& src, Marshal_ptr marshaler, ABI_type& dst);
	static void marshal_out (Any& src, Marshal_ptr marshaler, ABI_type& dst);
	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Any& dst);
};

}

typedef Nirvana::Type <Any>::C_var Any_var;
typedef Nirvana::Type <Any>::C_in Any_in;
typedef Nirvana::Type <Any>::C_out Any_out;
typedef Nirvana::Type <Any>::C_inout Any_inout;

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
void operator <<= (Any&, Exception&&);

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
Boolean operator >>= (const Any&, SystemException&);

}

#endif
