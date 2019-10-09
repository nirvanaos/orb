#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include "StringABI.h"


/*
namespace CORBA {
namespace Nirvana {

template <typename C>
class String_in :
	public std::basic_string <C>
{
public:
	String_in (const C* s)
	{
		if (s) {
			this->large_pointer (s);
			this->large_size (std::char_traits <C>::length (s));
		}
	}

	~String_in ()
	{
		this->reset ();
	}
};

template <typename C>
class String_var :
	public String_in <C>
{
public:
	String_var ()
	{}

	String_var (C* s)
	{

	}

	String_var (const C* s)
	{}
};

}

typedef Nirvana::String_in <char> String_in;
typedef Nirvana::String_in <wchar_t> WString_in;

// For compatibility with old C++ mapping specification
char* string_alloc (uint32_t len);
char* string_dup (const char* s);
void string_free (char* s);

}
*/
#endif
