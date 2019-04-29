// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include "EnvironmentImpl.h"
#include "T_ptr.h"
#include "RepositoryId.h"

namespace CORBA {
namespace Nirvana {

template <class I> class T_var;
template <class I> class T_out;
template <class I> class T_inout;
template <class I> class T_ref;

typedef T_ptr <Interface> Interface_ptr;
typedef T_var <Interface> Interface_var;
typedef T_out <Interface> Interface_out;

class Interface :
	public BridgeMarshal <Interface>
{
public:
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf);

	static void _release (Bridge <Interface>* itf);

	inline static T_ptr <Interface> _nil ();

	static Bridge <Interface>* unmarshal (Bridge <Interface>* bridge, const Char* interface_id);
};

template <>
class T_ptr <Interface>
{
public:
	T_ptr () :
		p_ (nullptr)
	{}

	T_ptr (Interface* p) :
		p_ (p)
	{}

	T_ptr (Bridge <Interface>* p) :
		p_ (static_cast <Interface*> (p))
	{}

	template <class I1>
	T_ptr (const T_ptr <I1>& src) :
		T_ptr (src.p_)
	{}

	operator BridgeMarshal <Interface>* () const
	{
		return p_;
	}

	Interface* operator -> () const
	{
		assert (p_);
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

inline T_ptr <Interface> Interface::_nil ()
{
	return T_ptr <Interface>::nil ();
}

}

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

namespace Nirvana {

//! T_var helper class
template <class I>
class T_var : public T_ptr <I>
{
public:
	T_var () :
		T_ptr <I> (I::_nil ())
	{}

	T_var (T_ptr <I> p) :
		T_ptr <I> (p)
	{}

	T_var (const T_var <I>& src) :
		T_ptr <I> (I::_duplicate (src))
	{}

	T_var (BridgeMarshal <I>* bridge) :
		T_ptr <I> (I::_nil ())
	{
		try {
			T_ptr <I>::operator = (bridge);
		} catch (...) {
			Interface::_release (bridge);
			throw;
		}
	}

	~T_var ()
	{
		release (*this);
	}

	T_var <I>& operator = (T_ptr <I> p)
	{
		reset (p);
		return *this;
	}

	T_var <I>& operator = (const T_var <I>& src)
	{
		if (&src != this) {
			reset (I::_nil ());
			T_ptr <I>::operator = (I::_duplicate (src));
		}
		return *this;
	}

	T_ptr <I> in () const
	{
		return *this;
	}

	T_inout <I>& inout ()
	{
		return *this;
	}

	T_out <I> out ()
	{
		return T_out <I> (*this);
	}

	T_ptr <I> _retn ()
	{
		T_ptr <I> p = *this;
		T_ptr <I>::operator = (T_ptr <I>::nil ());
		return p;
	}

protected:
	void reset (T_ptr <I> p)
	{
		release (*this);
		T_ptr <I>::operator = (p);
	}
};

template <class I>
class T_ref
{
public:
	T_ref (T_ptr <I>& p) :
		ptr_ (p->p_)
	{}

	~T_ref ()
	{
		if (ptr_) {
			try {
				I::unmarshal (ptr_);
			} catch (...) {
				release (ptr_);
				ptr_ = nullptr;
				throw;
			}
		}
	}

	operator BridgeMarshal <I>** ()
	{
		return ptr_;
	}

private:
	BridgeMarshal <I>*& ptr_;
};

template <class I>
class T_inout_base
{
public:
	T_inout_base (T_ptr <I>& rhs) :
		ptr_ (rhs)
	{}

	T_inout_base (T_var <I>& rhs) :
		ptr_ (rhs)
	{}

	T_inout_base (T_inout_base& rhs) :
		ptr_ (rhs.ptr_)
	{}

	T_inout_base (BridgeMarshal <I>*& bridge) :
		ptr_ (*reinterpret_cast <T_ptr <I>*> (&bridge))
	{}

	T_inout_base <I>& operator = (T_inout_base <I>& rhs)
	{
		ptr_ = rhs.ptr_;
		return *this;
	}

	T_inout_base <I>& operator = (T_var <I>& rhs)
	{
		ptr_ = I::_duplicate (rhs);
		return *this;
	}

	T_inout_base <I>& operator = (T_ptr <I> p)
	{
		ptr_ = p;
		return *this;
	}

	operator T_ptr <I>& ()
	{
		return ptr_;
	}

	operator T_ref <I> ()
	{
		return T_ref <I> (ptr_);
	}

	T_ptr <I>& ptr ()
	{
		return ptr_;
	}

	T_ptr <I> operator -> ()
	{
		return ptr_;
	}

protected:
	T_ptr <I>& ptr_;
};

/// T_out helper class
template <class I>
class T_out : public T_inout_base <I>
{
	typedef T_inout_base <I> Base;
public:
	T_out (T_ptr <I>& rhs) :
		Base (rhs)
	{
		Base::ptr_ = I::_nil ();
	}

	T_out (T_var <I>& rhs) :
		Base (rhs)
	{
		release (Base::ptr_);
	}

	T_out (T_out& rhs) :
		Base (rhs.ptr_)
	{}

	T_out (BridgeMarshal <I>*& bridge) :
		Base (bridge)
	{}

	T_out <I>& operator = (T_out <I>& rhs)
	{
		Base::operator = (rhs);
		return *this;
	}

	T_out <I>& operator = (T_var <I>& rhs)
	{
		Base::operator = (rhs);
		return *this;
	}

	T_out <I>& operator = (T_ptr <I> rhs)
	{
		Base::operator = (rhs);
		return *this;
	}
};

template <class I>
class T_inout : public T_inout_base <I>
{
	typedef T_inout_base <I> Base;
public:
	T_inout (T_ptr <I>& rhs) :
		Base (rhs)
	{}

	T_inout (T_var <I>& rhs) :
		Base (rhs)
	{}

	T_inout (T_inout& rhs) :
		Base (rhs.ptr_)
	{}

	T_inout (BridgeMarshal <I>*& bridge) :
		Base (bridge)
	{
		I::unmarshal (Base::ptr_);
	}

	T_inout <I>& operator = (T_inout <I>& rhs)
	{
		Base::operator = (rhs);
		return *this;
	}

	T_inout <I>& operator = (T_var <I>& rhs)
	{
		Base::operator = (rhs);
		return *this;
	}

	T_inout <I>& operator = (T_ptr <I> rhs)
	{
		Base::operator = (rhs);
		return *this;
	}
};

//! Client implementation template.
template <class T, class I> class Client
	: public T
{
	// Default implementation does not contain any methods.
};

template <class I>
class ClientBridge :
	public Bridge <I>
{
protected:
	Bridge <I>& _get_bridge (Environment& env)
	{
		return *this;
	}
};

//! Primary interface client implementation.
template <class I>
class ClientInterfacePrimary : 
	public Client <ClientBridge <I>, I>
{
public:
	typedef T_ptr <I> _ptr_type;

	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <I*> (Interface::_duplicate (obj));
	}

	static T_ptr <I> _nil ()
	{
		return T_ptr <I>::nil ();
	}

	static T_ptr <I> unmarshal (BridgeMarshal <I>* bridge)
	{
		return static_cast <I*> (Interface::unmarshal (bridge, Bridge <I>::interface_id_));
	}
};

//! ClientBase - How base client obtains pointer to bridge from primary.
template <class Primary, class Base>
class ClientBase
{
public:
	operator T_ptr <Base> ();

protected:
	Bridge <Base>* _get_bridge_ptr (Environment& env)
	{
		Primary& t = static_cast <Primary&> (*this);
		typename BridgeMarshal <Primary>:: template Wide <Base>::Func func = t._epv ().base;
		Bridge <Base>* ret = (func)(&t, Bridge <Base>::interface_id_, &env);
		env.check ();
		assert (!ret || RepositoryId::compatible (ret->_epv().interface.interface_id, Bridge <Base>::interface_id_));
		return ret;
	}

	Bridge <Base>& _get_bridge (Environment& env)
	{
		Bridge <Base>* ret = _get_bridge_ptr (env);
		if (!ret)
			throw MARSHAL ();
		return *ret;
	}
};

template <class Primary, class Base>
ClientBase <Primary, Base>::operator T_ptr <Base> ()
{
	Environment env;
	return _get_bridge_ptr (env);
}

//! Base interface client implementation.
//! Has specializations for Object and AbstractBase.
template <class Primary, class I>
class ClientInterfaceBase :
	public Client <ClientBase <Primary, I>, I>
{};

//! Base for client interface.
template <class Primary, class ... Bases>
class ClientInterface :
	public ClientInterfacePrimary <Primary>,
	public ClientInterfaceBase <Primary, Bases>...
{};

}
}

#endif
