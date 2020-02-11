// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_FORWARD_H_
#define NIRVANA_ORB_INTERFACE_FORWARD_H_

#include "T_ptr.h"

namespace CORBA {
namespace Nirvana {

typedef T_ptr <Interface> Interface_ptr;

class Interface :
	public BridgeMarshal <Interface>
{
public:
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf);

	static T_ptr <Interface> _duplicate (T_ptr <Interface> itf);

	static void _release (Bridge <Interface>* itf);

	inline static T_ptr <Interface> _nil ();

	static Bridge <Interface>* unmarshal (Bridge <Interface>* bridge, const Char* interface_id);

	static T_ptr <Interface> unmarshal (Bridge <Interface>* bridge);
};

template <>
class T_ptr <Interface>
{
public:
	T_ptr () :
		p_ (nullptr)
	{}

	T_ptr (const T_ptr& src) :
		p_ (src.p_)
	{}

	T_ptr (Interface* p) :
		p_ (p)
	{}

	template <class I>
	T_ptr (const T_ptr <I>& src) :
		p_ (static_cast <Interface*> (static_cast <Bridge <Interface>*> (src)))
	{}

	operator BridgeMarshal <Interface>* () const
	{
		return p_;
	}

	Interface* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	operator bool () const
	{
		return p_ != 0;
	}

	static T_ptr <Interface> nil ()
	{
		return T_ptr ((Interface*)nullptr);
	}

private:
	Interface* p_;
};

inline T_ptr <Interface> Interface::_duplicate (T_ptr <Interface> itf)
{
	return unmarshal (__duplicate (itf));
}

inline T_ptr <Interface> Interface::_nil ()
{
	return T_ptr <Interface>::nil ();
}

inline T_ptr <Interface> Interface::unmarshal (Bridge <Interface>* bridge)
{
	return static_cast <Interface*> (bridge);
}

} // Nirvana

//! CORBA::is_nil()
inline bool is_nil (Nirvana::Interface_ptr itf)
{
	return !itf;
}

//! CORBA::release()
inline void release (Nirvana::Interface_ptr itf)
{
	Nirvana::Interface::_release (itf);
}

} // CORBA

#endif
