// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include <Nirvana/Nirvana.h>
#include "BasicTypes.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBridge;

/// All interfaces derived from Interface class.
/// This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;

/// Intermediate part of an interface
template <class I> class Bridge;

/// All bridges must be derived from `Bridge <Interface>`.
template <>
class Bridge <Interface>
{
	Bridge (const Bridge&) = delete;
	Bridge& operator = (const Bridge&) = delete;
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

protected:
	Bridge (const EPV& epv) :
		epv_ref_ (epv)
	{}

protected:
	const EPV& epv_ref_;
};

/// Interface pointer template.
template <class I>
class T_ptr
{
public:
	T_ptr ()
		// Zeroinit skipped for performance
	{}

	T_ptr (I* p) :
		p_ (p)
	{}

	inline T_ptr (Bridge <I>* bridge);

	template <class I1>
	T_ptr (T_ptr <I1> src) :
		T_ptr (static_cast <Bridge <I>*> (src.p_))
	{}

	template <class I1>
	T_ptr& operator = (T_ptr <I1> src)
	{
		p_ = static_cast <Bridge <I>*> (src.p_);
		return *this;
	}

	operator Bridge <I>* () const
	{
		return static_cast <Bridge <I>*> (p_);
	}

	I* operator -> () const
	{
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

/// Interface - All client interfaces can be widened to it.
class Interface :
	public Bridge <Interface>
{
public:
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf);
	static void _release (Bridge <Interface>* itf);

	static Interface_ptr _nil ()
	{
		return Interface_ptr::nil ();
	}

	static Interface_ptr adopt (Bridge <Interface>* bridge)
	{
		return static_cast <Interface*> (bridge);
	}

protected:
	static void adopt (const Bridge <Interface>* bridge, const Char* interface_id);
};

template <class I>
T_ptr <I>::T_ptr (Bridge <I>* bridge) :
	T_ptr (I::adopt (bridge))
{}

}

/// CORBA::is_nil()
inline bool is_nil (Nirvana::Interface_ptr itf)
{
	return !itf;
}

/// CORBA::release()
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

	T_var (Bridge <I>* bridge) :
		T_ptr <I> (I::_nil ())
	{
		try {
			T_ptr::operator = (I::adopt (bridge));
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
		try {
			I::adopt (ptr_);
		} catch (...) {
			release (ptr_);
			throw;
		}
	}

	operator Bridge <I>*& ()
	{
		return ptr_;
	}

private:
	Bridge <I>*& ptr_;
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

	T_inout_base (Bridge <I>*& bridge) :
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

	T_out (Bridge <I>*& bridge) :
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

	T_inout (Bridge <I>*& bridge) :
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

/// ClientBase - How client obtains pointer to Bridge

template <class T, class I>
class ClientBase
{
protected:
	Bridge <I>& _bridge ()
	{
		return static_cast <Bridge <I>&> (static_cast <T&> (*this));
	}
};

/// Client must derive from ClientBase
template <class T, class I> class Client;

/// ClientInterface - Base template for client interfaces

template <class I>
class ClientInterfaceBase :
	public Bridge <I>,
	public Client <I, I>
{
public:
	operator Interface& ()
	{
		return *static_cast <Interface*> (static_cast <Bridge <Interface>*> (static_cast <Bridge <I>*>(this)));
	}

	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <I*> (Interface::_duplicate (obj));
	}

	static T_ptr <I> _nil ()
	{
		return T_ptr <I>::nil ();
	}

	T_ptr <I> adopt (Bridge <I>* bridge)
	{
		Interface::adopt (bridge, Bridge <I>::interface_id_);
		return static_cast <I*> (bridge);
	}
};

}

}

#endif
