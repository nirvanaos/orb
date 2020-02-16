// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_FORWARD_H_
#define NIRVANA_ORB_INTERFACE_FORWARD_H_

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

typedef I_ptr <Interface> Interface_ptr;

class Interface :
	public BridgeMarshal <Interface>
{
public:
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf);

	static I_ptr <Interface> _duplicate (I_ptr <Interface> itf);

	static void _release (Bridge <Interface>* itf);

	inline static I_ptr <Interface> _nil ();

	static Bridge <Interface>* _check (Bridge <Interface>* bridge, const Char* interface_id);

	static I_ptr <Interface> _check (Bridge <Interface>* bridge);
};

template <>
class I_ptr <Interface>
{
public:
	I_ptr () :
		p_ (nullptr)
	{}

	I_ptr (const I_ptr& src) :
		p_ (src.p_)
	{}

	I_ptr (Interface* p) :
		p_ (p)
	{}

	template <class I>
	I_ptr (const I_ptr <I>& src) :
		p_ (static_cast <Interface*> (static_cast <Bridge <Interface>*> (src)))
	{}

	operator BridgeMarshal <Interface>* () const
	{
		return p_;
	}

	operator Bridge <Interface>* () const
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

	static I_ptr <Interface> nil ()
	{
		return I_ptr ((Interface*)nullptr);
	}

private:
	Interface* p_;
};

inline I_ptr <Interface> Interface::_duplicate (I_ptr <Interface> itf)
{
	return _check (__duplicate (itf));
}

inline I_ptr <Interface> Interface::_nil ()
{
	return I_ptr <Interface>::nil ();
}

inline I_ptr <Interface> Interface::_check (Bridge <Interface>* bridge)
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
