#ifndef NIRVANA_ORB_STRINGBASE_H_
#define NIRVANA_ORB_STRINGBASE_H_

#include "ABI_String.h"
#include <string.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringBase : protected ABI <StringT <C> >
{
public:
	StringBase (const C*);

	const ABI <StringT <C> >* operator & () const
	{
		return this;
	}

protected:
	StringBase ()
	{}

private:
	static size_t _length (const C* s);
};

template <typename C>
StringBase <C>::StringBase (const C* s)
{
	if (s) {
		size_t cc = _length (s);
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

template <> inline
size_t StringBase <Char>::_length (const Char* s)
{
	return strlen (s);
}

template <> inline
size_t StringBase <WChar>::_length (const WChar* s)
{
	return wcslen (s);
}

}

typedef const Nirvana::StringBase <Char>& String_in;
typedef const Nirvana::StringBase <WChar>& WString_in;

}

#endif
