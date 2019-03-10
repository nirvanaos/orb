// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include <Nirvana/Nirvana.h>
#include "Exception.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBridge;

//! All interfaces derives from Interface class.
//! This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;

//! Intermediate part of an interface, bridge between servant and client.
template <class I> class Bridge;

//! All bridges must derive from `Bridge <Interface>'.
template <>
class Bridge <Interface>
{
public:
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

	//! All client interfaces can be widened to Interface.
	inline operator Interface& ();

protected:
	Bridge (const EPV& epv) :
		epv_ref_ (epv)
	{}

protected:
	const EPV& epv_ref_;
};

//! Client end of the bridge.
template <class I> class ClientBridge;

//! Interface pointer template.
template <class I>
class T_ptr
{
public:
	T_ptr ()
		// Zeroinit skipped for performance
	{}

	template <class T>
	T_ptr (T* p) :
		p_ (p ? &static_cast <I&> (*p) : nullptr)
	{}

	template <class I1>
	T_ptr (const T_ptr <I1> src) :
		T_ptr (src.p_)
	{}

	operator ClientBridge <I>* () const
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

	static T_ptr nil ()
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

template <>
class ClientBridge <Interface> :
	public Bridge <Interface>
{
public:
	static Interface_ptr _nil ()
	{
		return Interface_ptr::nil ();
	}

	inline static Interface* adopt (ClientBridge <Interface>* bridge);
};

class Interface :
	public ClientBridge <Interface>
{
public:
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf);

	static void _release (Bridge <Interface>* itf);

	static Bridge <Interface>* adopt (Bridge <Interface>* bridge, const Char* interface_id);
};

Bridge <Interface>::operator Interface& ()
{
	return static_cast <Interface&> (*this);
}

inline Interface* ClientBridge <Interface>::adopt (ClientBridge <Interface>* bridge)
{
	return static_cast <Interface*> (bridge);
}

template <class I>
class ClientBridge :
	public Bridge <I>
{
public:
	operator I& ()
	{
		return *adopt (this);
	}

	static I* adopt (ClientBridge <I>* bridge)
	{
		return static_cast <I*> (Interface::adopt (bridge, Bridge <I>::interface_id_));
	}

	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <I*> (Interface::_duplicate (obj));
	}

	static T_ptr <I> _nil ()
	{
		return T_ptr <I>::nil ();
	}
};

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

/// T_var helper class
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

	T_var (ClientBridge <I>* bridge) :
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
				ptr_ = ptr_->operator I&(); // Adopt
			} catch (...) {
				release (ptr_);
				ptr_ = nullptr;
				throw;
			}
		}
	}

	operator ClientBridge <I>*& ()
	{
		return ptr_;
	}

private:
	ClientBridge <I>*& ptr_;
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

	T_inout_base (ClientBridge <I>*& bridge) :
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

	T_out (ClientBridge <I>*& bridge) :
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

	T_inout (ClientBridge <I>*& bridge) :
		Base (bridge)
	{
		I::adopt (Base::ptr_);
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

//! ClientBase - How base client obtains pointer to bridge from primary.
template <class Primary, class I> class ClientBase;

//! Client implementation template.
template <class T, class I> class Client;

//! Primary interface client implementation.
template <class I>
using ClientInterfacePrimary = Client <ClientBridge <I>, I>;

//! Base interface client implementation.
template <class Primary, class I>
using ClientInterfaceBase = Client <ClientBase <Primary, I>, I>;

}
}

#endif
