// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include <Nirvana/Nirvana.h>
#include "Environment.h"
#include "Exception.h"
#include "RepositoryId.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBridge;

//! All interfaces derives from Interface class.
//! This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;

//! Intermediate part of an interface, bridge between servant and client.
template <class I> class Bridge;

//! All bridges indirectly derives from `Bridge <Interface>'.
template <>
class Bridge <Interface>
{
public:
	Bridge& operator = (const Bridge&)
	{
		return *this;	// Do nothing
	}

	struct EPV
	{
		const Char* interface_id;
		Bridge <Interface>* (*duplicate) (Bridge <Interface>*, EnvironmentBridge*);
		void (*release) (Bridge <Interface>*);
	};

	const EPV& _epv () const
	{
		return epv_ref_;
	}

protected:
	Bridge (const EPV& epv) :
		epv_ref_ (epv)
	{}

protected:
	const EPV& epv_ref_;
};

//! The bridge which was passed from a different binary file.
//! We can't be sure that the interface version is the same as for current binary.
//! To obtain the interface pointer we have to unmarshal the bridge pointer.
//! Each Bridge <I> derives from BridgeMarshal <I>.
template <class I>
class BridgeMarshal :
	public Bridge <Interface>
{
public:
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, const Char*, EnvironmentBridge*);
	};

protected:
	BridgeMarshal (const EPV& epv) :
		Bridge <Interface> (epv)
	{}
};

#define BASE_STRUCT_ENTRY(type, name) Wide < type>::Func name;\
operator const Wide < type>::Func () const { return name; }

//! Interface pointer template.
template <class I>
class T_ptr
{
public:
	T_ptr ()
	{} // Zero init skipped for performance

	T_ptr (I* p) :
		p_ (p)
	{}

	T_ptr (Bridge <I>* p) :
		p_ (static_cast <I*> (p))
	{}

	T_ptr (BridgeMarshal <I>* p) :
		T_ptr (I::unmarshal (p))
	{}

	T_ptr (const T_ptr <I>& src) :
		p_ (src.p_)
	{}

	template <class I1>
	T_ptr (const T_ptr <I1>& src, bool check_nil = true)
	{
		if (src.p_) {
			*this = src.p_->operator T_ptr ();
			if (check_nil && !p_)
				throw MARSHAL ();
		} else
			p_ = nullptr;
	}

	operator Bridge <I>* () const
	{
		return p_;
	}

	I* operator -> () const
	{
		assert (p_);
		return p_;
	}

	operator bool () const
	{
		return p_ != 0;
	}

	static T_ptr <I> nil ()
	{
		return T_ptr ((I*)nullptr);
	}

private:
	template <class I1> friend class T_ptr;

private:
	I* p_;
};

template <class I> class T_var;
template <class I> class T_out;
template <class I> class T_inout;
template <class I> class T_ref;

typedef ::CORBA::Nirvana::T_ptr <Interface> Interface_ptr;
typedef ::CORBA::Nirvana::T_var <Interface> Interface_var;
typedef ::CORBA::Nirvana::T_out <Interface> Interface_out;

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

	operator BridgeMarshal <I>*& ()
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
template <class T, class I> class Client;

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
