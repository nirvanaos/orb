#ifndef NIRVANA_ORB_STRINGMANAGER_C_H_
#define NIRVANA_ORB_STRINGMANAGER_C_H_

#include "Interface_c.h"

namespace CORBA {
namespace Nirvana {

template <class C> class StringManager;

template <class C>
class Bridge <StringManager <C> > :
	public BridgeMarshal <StringManager <C> >
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			C* (*string_alloc) (Bridge <StringManager <C> >*, ULong, EnvironmentBridge*);
			C* (*string_dup) (Bridge <StringManager <C> >*, const C*, EnvironmentBridge*);
			void (*string_free) (Bridge <StringManager <C> >*, C*, EnvironmentBridge*);
			C* (*at) (Bridge <StringManager <C> >*, C**, ULong, EnvironmentBridge*);
			// TODO: Extend for the full support of std::basic_string operations.
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <StringManager <C> > (epv.interface)
	{}
};

template <class T, class C>
class Client <T, StringManager <C> > :
	public T
{
public:
	C* string_alloc (ULong len);
	C* string_dup (const C*);
	void string_free (C*);
	C& at (C*& s, ULong index);
};

template <class T, class C>
C* Client <T, StringManager <C> >::string_alloc (ULong len)
{
	Environment _env;
	Bridge <StringManager <C> >& _b (T::_get_bridge (_env));
	C* _ret = (_b._epv ().epv.string_alloc) (&_b, len, &_env);
	_env.check ();
	return _ret;
}

template <class T, class C>
C* Client <T, StringManager <C> >::string_dup (const C* s)
{
	Environment _env;
	Bridge <StringManager <C> >& _b (T::_get_bridge (_env));
	C* _ret = (_b._epv ().epv.string_dup) (&_b, s, &_env);
	_env.check ();
	return _ret;
}

template <class T, class C>
void Client <T, StringManager <C> >::string_free (C* s)
{
	Environment _env;
	Bridge <StringManager <C> >& _b (T::_get_bridge (_env));
	(_b._epv ().epv.string_free) (&_b, s, &_env);
	_env.check ();
}

template <class T, class C>
C& Client <T, StringManager <C> >::at (C*& s, ULong index)
{
	Environment _env;
	Bridge <StringManager <C> >& _b (T::_get_bridge (_env));
	C* _ret = (_b._epv ().epv.at) (&_b, &s, index, &_env);
	_env.check ();
	return *_ret;
}

template <class C>
class StringManager : public ClientInterface <StringManager <C> >
{
public:
	static T_ptr <StringManager <C> > singleton ()
	{
		return singleton_;
	}

	static Bridge <StringManager <C> >* const singleton_;
};

}
}

#endif
