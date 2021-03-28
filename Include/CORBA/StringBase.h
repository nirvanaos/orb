#ifndef NIRVANA_ORB_STRINGBASE_H_
#define NIRVANA_ORB_STRINGBASE_H_

#include "ABI_String.h"
#include <type_traits>
#include <string.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringBase : protected ABI <StringT <C> >
{
public:
#ifdef NIRVANA_C11
	template <size_t cc>
	StringBase (C const (&s) [cc])
	{
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc - 1);
		this->allocated (0);
	}

	template <typename S, typename = typename std::enable_if <std::is_convertible <S, const C*>::value && !std::is_array <S>::value>::type>
	StringBase (S s)
	{
		const C* p = s;
		if (p) {
			size_t cc = _length (p);
			this->large_pointer (const_cast <C*> (p));
			this->large_size (cc);
			this->allocated (0);
		} else
			this->reset ();
	}
#else
	StringBase (const C* p)
	{
		if (p) {
			size_t cc = _length (p);
			this->large_pointer (const_cast <C*> (p));
			this->large_size (cc);
			this->allocated (0);
		} else
			this->reset ();
	}
#endif

	StringBase (const C* p, size_t cc)
	{
		if (p && cc) {
			this->large_pointer (const_cast <C*> (p));
			this->large_size (cc);
			this->allocated (0);
		} else
			this->reset ();
	}

	StringBase (nullptr_t)
	{
		this->reset ();
	}

	const ABI <StringT <C> >* operator & () const
	{
		return this;
	}

	// For member assignments
	operator const StringT <C>& () const;

protected:
	StringBase ()
	{}

private:
	static size_t _length (const C* s);
};

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
