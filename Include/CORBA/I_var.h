#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_out;

template <class I>
class I_var_base
{
public:
	I_var_base () NIRVANA_NOEXCEPT :
		p_ (0)
	{}

	I_var_base (I* p) NIRVANA_NOEXCEPT :
		p_ (p)
	{}

	I_var_base (const I_ptr_base <I>& p) NIRVANA_NOEXCEPT :
		p_ (p.p_)
	{}

	I_var_base (const I_var_base <I>& src) :
		p_ (static_cast <I*> (interface_duplicate (src.p_)))
	{}

	~I_var_base () NIRVANA_NOEXCEPT
	{
		release (I_ptr <I> (p_));
	}

	operator bool () const
	{
		return p_ != 0;
	}

	I_in <I> in () const;

protected:
	void reset (I* p) NIRVANA_NOEXCEPT
	{
		release (I_ptr <I> (p_));
		p_ = p;
	}

protected:
	friend class I_ptr_base <I>;
	friend class I_in <I>;
	friend class I_inout <I>;

	I* p_;
};

template <class I> inline
I_ptr_base <I>::I_ptr_base (const I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
}

template <class I> inline
I_ptr_base <I>& I_ptr_base <I>::operator = (const I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
	return *this;
}

template <class I> inline
void I_ptr_base <I>::move_from (I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
	var.p_ = 0;
}

//! I_var helper class for interface
template <class I>
class I_var : public I_var_base <I>
{
public:
	I_var () NIRVANA_NOEXCEPT :
		I_var_base <I> ()
	{}

	I_var (I* p) NIRVANA_NOEXCEPT :
		I_var_base <I> (p)
	{}

	I_var (const I_ptr <I>& ptr) NIRVANA_NOEXCEPT :
		I_var_base <I> (ptr)
	{}

	I_var (const I_var <I>& src) :
		I_var_base <I> (src)
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		I_var_base <I> (src.p_)
	{
		src.p_ = 0;
	}

	I_var& operator = (const I_ptr <I>& p) NIRVANA_NOEXCEPT
	{
		I_var_base <I>::reset (p.p_);
		return *this;
	}

	I_var& operator = (const I_var& src)
	{
		if (&src != this) {
			I_var_base <I>::reset (0);
			I_var_base <I>::p_ = static_cast <I*> (interface_duplicate (src.p_));
		}
		return *this;
	}

	I_var& operator = (I_var&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			I_var_base <I>::reset (src.p_);
			src.p_ = 0;
		}
		return *this;
	}

	I* operator -> () const
	{
		if (!I_var_base <I>::p_)
			::Nirvana::throw_INV_OBJREF ();
		return I_var_base <I>::p_;
	}

	inline I_inout <I> inout ();
	inline I_out <I> out ();

	I_ptr <I> _retn () NIRVANA_NOEXCEPT
	{
		I_ptr <I> p (I_var_base <I>::p_);
		I_var_base <I>::p_ = 0;
		return p;
	}

	operator Bridge <I>* () const NIRVANA_NOEXCEPT
	{
		return static_cast <Bridge <I>*> (I_var_base <I>::p_);
	}

	/// Destroying conversion to the ABI return value.
	/// When servant returns I_var, skeleton have to return Interface*
	/// and reset the I_var to avoid object deletion on return.
	operator Interface* () NIRVANA_NOEXCEPT
	{
		Interface* p = I_var_base <I>::p_;
		I_var_base <I>::p_ = 0;
		return p;
	}
};

template <>
class I_var <Interface> : public I_var_base <Interface>
{
public:
	I_var () NIRVANA_NOEXCEPT :
		I_var_base <Interface> ()
	{}

	I_var (Interface* p) NIRVANA_NOEXCEPT :
		I_var_base <Interface> (p)
	{}

	I_var (const I_ptr <Interface>& ptr) NIRVANA_NOEXCEPT :
		I_var_base <Interface> (ptr)
	{}

	I_var (const I_var <Interface>& src) :
		I_var_base <Interface> (src)
	{}

	I_var (I_var <Interface>&& src) NIRVANA_NOEXCEPT :
		I_var_base <Interface> (src.p_)
	{
		src.p_ = 0;
	}

	I_var& operator = (const I_ptr <Interface>& p) NIRVANA_NOEXCEPT
	{
		I_var_base <Interface>::reset (p.p_);
		return *this;
	}

	I_var& operator = (const I_var& src)
	{
		if (&src != this) {
			I_var_base <Interface>::reset (0);
			I_var_base <Interface>::p_ = interface_duplicate (src.p_);
		}
		return *this;
	}

	I_var& operator = (I_var&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			I_var_base <Interface>::reset (src.p_);
			src.p_ = 0;
		}
		return *this;
	}

	inline I_inout <Interface> inout ();
	inline I_out <Interface> out ();

	I_ptr <Interface> _retn () NIRVANA_NOEXCEPT
	{
		I_ptr <Interface> p (I_var_base <Interface>::p_);
		I_var_base <Interface>::p_ = 0;
		return p;
	}

	/// Destroying conversion to the ABI return value.
	/// When servant returns I_var, skeleton have to return Interface*
	/// and reset the I_var to avoid object deletion on return.
	operator Interface* () NIRVANA_NOEXCEPT
	{
		Interface* p = I_var_base <Interface>::p_;
		I_var_base <Interface>::p_ = 0;
		return p;
	}
};

}
}

#endif
