#include <CORBA/String.h>

namespace CORBA {

// The string_alloc, string_dup, and string_free functions may not throw exceptions. See the C++ mapping standard.

Char* string_alloc (ULong len)
{
	Nirvana::Bridge <Nirvana::StringManager <Char> >* br = Nirvana::StringManager <Char>::singleton ();
	return (br->_epv ().epv.string_alloc) (br, len, nullptr);
}

Char* string_dup (const Char* s)
{
	Nirvana::Bridge <Nirvana::StringManager <Char> >* br = Nirvana::StringManager <Char>::singleton ();
	return (br->_epv ().epv.string_dup) (br, s, nullptr);
}

void string_free (Char* s)
{
	Nirvana::Bridge <Nirvana::StringManager <Char> >* br = Nirvana::StringManager <Char>::singleton ();
	(br->_epv ().epv.string_free) (br, s, nullptr);
}

WChar* wstring_alloc (ULong len)
{
	Nirvana::Bridge <Nirvana::StringManager <WChar> >* br = Nirvana::StringManager <WChar>::singleton ();
	return (br->_epv ().epv.string_alloc) (br, len, nullptr);
}

WChar* wstring_dup (const WChar* s)
{
	Nirvana::Bridge <Nirvana::StringManager <WChar> >* br = Nirvana::StringManager <WChar>::singleton ();
	return (br->_epv ().epv.string_dup) (br, s, nullptr);
}

void wstring_free (WChar* s)
{
	Nirvana::Bridge <Nirvana::StringManager <WChar> >* br = Nirvana::StringManager <WChar>::singleton ();
	(br->_epv ().epv.string_free) (br, s, nullptr);
}

}
