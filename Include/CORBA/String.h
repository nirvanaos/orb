#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
StringBase <C>::StringBase (const C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

#ifdef NIRVANA_C11

template <typename C>
template <class T, class A, typename>
StringBase <C>::StringBase (const std::basic_string <C, T, A>& s)
{
	if (!s.empty ()) {
		this->large_pointer (const_cast <C*> (s.data ()));
		this->large_size (s.length ());
		this->allocated (0);
	} else
		this->reset ();
}

#endif

typedef StringT <Char> String;
typedef StringT <WChar> WString;

template <typename C>
struct Type <StringT <C> > : TypeVarLen <StringT <C>, CHECK_STRINGS>
{
	typedef TypeVarLen <StringT <C>, CHECK_STRINGS> Base;
	typedef ABI <StringT <C> > ABI_type;

	static void check (const ABI_type& s);

	typedef typename Base::ABI_out ABI_out;

	typedef const StringBase <C>& C_in;

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (StringT <C>& s) :
			Base::C_inout (s)
		{}

		ABI_type* operator & () const
		{
			return &static_cast <ABI_type&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (StringT <C>& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static StringT <C>& out (ABI_out p)
	{
		StringT <C>& val = Base::out (p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}
};

template <typename C>
void Type <StringT <C> >::check (const ABI_type& s)
{
	// Do some check
	const C* p;
	size_t cc;
	if (s.is_large ()) {
		p = s.large_pointer ();
		cc = s.large_size ();
		CORBA::Nirvana::_check_pointer (p);
		if (cc > s.large_capacity () || !StringT <C>::memory ()->is_readable (p, (cc + 1) * sizeof (C)))
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = s.small_pointer ();
		cc = s.small_size ();
		if (cc > ABI_type::SMALL_CAPACITY)
			::Nirvana::throw_BAD_PARAM ();
	}
	if (p [cc])
		::Nirvana::throw_BAD_PARAM (); // Not zero-terminated
}

template <typename C>
using TypeString = Type <StringT <C> >;

template <typename C>
using String_in = typename TypeString <C>::C_in;

template <typename C>
using String_out = typename TypeString <C>::C_out;

template <typename C>
using String_inout = typename TypeString <C>::C_inout;

}

typedef Nirvana::String_in <Char> String_in;
typedef Nirvana::String_in <WChar> WString_in;

typedef Nirvana::String_out <Char> String_out;
typedef Nirvana::String_out <WChar> WString_out;

typedef Nirvana::String_inout <Char> String_inout;
typedef Nirvana::String_inout <WChar> WString_inout;

// String_var and WString_var classes are defined in String_compat.h

}

#endif
