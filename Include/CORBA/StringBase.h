#ifndef NIRVANA_ORB_STRINGBASE_H_
#define NIRVANA_ORB_STRINGBASE_H_

#include "ABI_String.h"
#include "BasicTypes.h"

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
};

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

}

typedef const Nirvana::StringBase <Char>& String_in;
typedef const Nirvana::StringBase <WChar>& WString_in;

}

#endif
