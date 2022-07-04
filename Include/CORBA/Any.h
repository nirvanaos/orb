/// \file
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
#pragma once

#include "TypeVarLen.h"
#include "ABI_Any.h"
#include "String.h"
#include <Nirvana/basic_string.h>
//#include "Fixed.h"
#include "T_var.h"

namespace CORBA {

class Any : private Internal::ABI <Any>
{
	typedef Internal::ABI <Any> ABI;
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

	Internal::I_ptr <TypeCode> type () const
	{
		return ABI::type ();
	}

	void type (Internal::I_ptr <TypeCode> alias);

	void* data ();

	const void* data () const
	{
		return const_cast <Any&> (*this).data ();
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

	void copy_from (Internal::I_ptr <TypeCode> tc, const void* val);
	void move_from (Internal::I_ptr <TypeCode> tc, void* val);

private:
	friend struct Internal::Type <Any>;

	void copy_from (const Any& src);
	void* prepare (Internal::I_ptr <TypeCode> tc);
	void set_type (Internal::I_ptr <TypeCode> tc);
	void set_type (Internal::I_ref <TypeCode>&& tc);

	// these functions are private and not implemented
	// hiding these causes compile-time errors for
	// unsigned char
	void operator <<= (unsigned char);
	Boolean operator >>= (unsigned char &) const;
};

namespace Internal {

template <>
struct Type <Any> : public TypeVarLen <Any>
{
	typedef TypeVarLen <Any> Base;
	typedef typename Base::ABI ABI;

	static void check (const ABI& any);

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

	static I_ptr <TypeCode> type_code ()
	{
		return _tc_any;
	}

	static void marshal_in (const Any& src, IORequest_ptr rq);
	static void marshal_out (Any& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, Any& dst);
};

}

#ifdef LEGACY_CORBA_CPP
typedef Internal::T_var <Any> Any_var;
#endif

template <typename T> inline
void operator <<= (Any& a, const T& v)
{
	a.copy_from (Internal::Type <T>::type_code (), &v);
}

template <typename T> inline
void operator <<= (Any& a, T&& v)
{
	a.move_from (Internal::Type <T>::type_code (), &v);
}

template <typename T> inline
Boolean operator >>= (const Any& a, T& v)
{
	if (Internal::Type <T>::type_code ()->equal (a.type ())) {
		v = *(T*)a.data ();
		return true;
	}
	return false;
}

inline
void operator <<= (Any& dst, const Any& src)
{
	dst = src;
}
inline
void operator <<= (Any& dst, Any&& src)
{
	dst = std::move (src);
}

void operator <<= (Any&, const Exception&);
void operator <<= (Any&, Exception&&);

inline
Boolean operator >>= (const Any& src, Any& dst)
{
	dst = src;
	return true;
}

Boolean operator >>= (const Any&, SystemException&);

}

#endif
