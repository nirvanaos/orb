#ifndef NIRVANA_ORB_I_PTR_H_
#define NIRVANA_ORB_I_PTR_H_

#include "Bridge.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

template <class I> class I_var;
template <class I> class I_in;
template <class I> class I_inout;

#ifdef _DEBUG
const uintptr_t UNINITIALIZED_PTR = 1;
#endif

template <class I> class I_var_base;

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

	I_ptr_base& operator = (const I_var <I>& var) NIRVANA_NOEXCEPT;

	operator bool () const
	{
		return p_ != 0;
	}

protected:
	void move_from (I_var <I>& var) NIRVANA_NOEXCEPT;

protected:
	friend class I_var_base <I>;
	friend class I_var <I>;
	friend class I_in <I>;
	friend class I_inout <I>;

	I* p_;
};

//! Interface pointer template.
template <class I>
class I_ptr : public I_ptr_base <I>
{
	/// Obtaining I_ptr directly from a servant pointer is prohibited
	I_ptr (Bridge <I>* p)
#ifdef NIRVANA_C11
		= delete
#endif
		;

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
			I_ptr_base <I>::p_ = 0;
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
		I_ptr_base <I>::move_from (var);
	}

	I_ptr& operator = (const I_ptr <I>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <I>::operator = (src);
		return *this;
	}

	I_ptr& operator = (const I_var <I>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <I>::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	operator Bridge <I>* () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)I_ptr_base <I>::p_);
		return static_cast <Bridge <I>*> (I_ptr_base <I>::p_);
	}

	I* operator -> () const
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)I_ptr_base <I>::p_);
		if (!I_ptr_base <I>::p_)
			::Nirvana::throw_INV_OBJREF ();
		return I_ptr_base <I>::p_;
	}

private:
	template <class I1> friend class I_ptr;
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

	I_ptr (const I_var <Interface>& var) NIRVANA_NOEXCEPT :
		I_ptr_base <Interface> (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <Interface>&& var) NIRVANA_NOEXCEPT
	{
		I_ptr_base <Interface>::move_from (var);
	}

	I_ptr& operator = (const I_ptr <Interface>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <Interface>::operator = (src);
		return *this;
	}

	I_ptr& operator = (const I_var <Interface>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <Interface>::operator = (src);
		return *this;
	}

	operator Interface* () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)I_ptr_base <Interface>::p_);
		return I_ptr_base <Interface>::p_;
	}
};

typedef I_ptr <Interface> Interface_ptr;

template <class I>
struct StaticInterface
{
	Bridge <I>* itf;

	operator CORBA::Nirvana::I_ptr <I> () const
	{
		return static_cast <I*> (itf);
	}

	I* operator -> () const
	{
		if (!itf)
			::Nirvana::throw_INV_OBJREF ();
		return static_cast <I*> (itf);
	}
};

}

/// CORBA::release
template <class I> inline
void release (const Nirvana::I_ptr <I>& ptr)
{
	Nirvana::interface_release (static_cast <Nirvana::Bridge <I>*> (ptr));
}

template <> inline
void release (const Nirvana::I_ptr <Nirvana::Interface>& ptr)
{
	Nirvana::interface_release (ptr);
}

}

#endif
