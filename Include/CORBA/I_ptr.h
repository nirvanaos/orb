#ifndef NIRVANA_ORB_I_PTR_H_
#define NIRVANA_ORB_I_PTR_H_

#include "Bridge.h"
#include <Nirvana/throw_exception.h>
#include <assert.h>

namespace CORBA {
namespace Nirvana {

template <class I> class I_ptr;
template <class I> class I_var;
template <class I> class I_inout;
template <class I> class I_ret;
template <class I> class ClientInterfacePrimary;

#ifdef _DEBUG
const uintptr_t UNINITIALIZED_PTR = 1;
#endif

template <class I>
class I_ptr_base
{
public:
	/// Zero init skipped for performance
	I_ptr_base () NIRVANA_NOEXCEPT
#ifdef _DEBUG
		: p_ ((I*)UNINITIALIZED_PTR)
#endif
	{}

	I_ptr_base (I* p) NIRVANA_NOEXCEPT :
		p_ (p)
	{}

	I_ptr_base (const I_ptr_base <I>& src) NIRVANA_NOEXCEPT :
		p_ (src.p_)
	{}

	I_ptr_base (const I_var <I>& var) NIRVANA_NOEXCEPT;

	I_ptr_base& operator = (const I_ptr_base <I>& src) NIRVANA_NOEXCEPT
	{
		p_ = src.p_;
		return *this;
	}

	I* operator -> () const
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	explicit operator bool () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		return p_ != 0;
	}

	explicit operator bool () NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)p_);
		return p_ != 0;
	}

protected:
	void move_from (I_var <I>& var) NIRVANA_NOEXCEPT;

protected:
	friend class I_var <I>;
	friend class I_inout <I>;
	template <class I1> friend class I_ptr;

	I* p_;
};

//! Interface pointer template.
template <class I>
class I_ptr : public I_ptr_base <I>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	I_ptr (I* p) NIRVANA_NOEXCEPT :
		I_ptr_base <I> (p)
	{}

	I_ptr (const I_ptr <I>& src) NIRVANA_NOEXCEPT :
		I_ptr_base <I> (src)
	{}

	template <class I1>
	I_ptr (const I_ptr <I1>& src)
	{
		if (src.p_)
			*this = static_cast <I_ptr> (*src.p_);
		else
			this->p_ = 0;
	}

	template <class I1>
	I_ptr (const I_var <I1>& src) :
		I_ptr (I_ptr <I1> (src))
	{}

	I_ptr (const I_var <I>& var) NIRVANA_NOEXCEPT :
		I_ptr_base <I> (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <I>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr <I>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <I>::operator = (src);
		return *this;
	}

	Bridge <I>* operator & () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return static_cast <Bridge <I>*> (this->p_);
	}
};

template <>
class I_ptr <Interface> : public I_ptr_base <Interface>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	I_ptr (Interface* p) NIRVANA_NOEXCEPT :
		I_ptr_base <Interface> (p)
	{}

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base <Interface> (src)
	{}

	template <class I>
	I_ptr (const I_ptr <I>& src) :
		I_ptr_base <Interface> (src.p_)
	{}

	I_ptr (const I_var <Interface>& var) NIRVANA_NOEXCEPT :
		I_ptr_base <Interface> (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Interface_var func ();
	///    Interface_ptr obj = func ();
	I_ptr (I_var <Interface>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr <Interface>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <Interface>::operator = (src);
		return *this;
	}

	Interface* operator & () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return this->p_;
	}
};

typedef I_ptr <Interface> Interface_ptr;

inline I_ptr <Interface> Interface::_nil ()
{
	return I_ptr <Interface> ((Interface*)nullptr);
}

template <class I>
struct StaticI_ptr
{
	Bridge <I>* itf;

	operator I_ptr <I> () const
	{
		return static_cast <I*> (itf);
	}

	I* operator -> () const
	{
		if (!itf)
			::Nirvana::throw_INV_OBJREF ();
		return static_cast <I*> (itf);
	}

	StaticI_ptr& operator = (I_ptr <I> ptr)
	{
		itf = &ptr;
		return *this;
	}
};

}

/// CORBA::release
template <class I> inline
void release (const Nirvana::I_ptr <I>& ptr)
{
	Nirvana::interface_release (&ptr);
}

/// CORBA::is_nil()
template <class I> inline
bool is_nil (const Nirvana::I_ptr <I>& ptr)
{
	return !ptr;
}

}

#endif
