#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_out;

//! I_var helper class for interface
template <class I>
class I_var
{
public:
	I_var () NIRVANA_NOEXCEPT :
		p_ (0)
	{}

	I_var (I* p) NIRVANA_NOEXCEPT :
		p_ (p)
	{}

	I_var (const I_ptr <I>& p) NIRVANA_NOEXCEPT :
		p_ (p.p_)
	{}

	I_var (const I_var <I>& src) :
		p_ (static_cast <I*> (interface_duplicate (src.p_)))
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		p_ (src.p_)
	{
		src.p_ = 0;
	}

	~I_var () NIRVANA_NOEXCEPT
	{
		interface_release (p_);
	}

	I_var& operator = (const I_ptr <I>& p) NIRVANA_NOEXCEPT
	{
		reset (p.p_);
		return *this;
	}

	I_var& operator = (const I_var& src)
	{
		if (src.p_ != this->p_)
			operator = (static_cast <I*> (interface_duplicate (src.p_)));
		return *this;
	}

	I_var& operator = (I_var&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (src.p_);
			src.p_ = 0;
		}
		return *this;
	}

	inline I_inout <I> inout ();
	inline I_out <I> out ();

	I* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	operator bool () const
	{
		return p_ != 0;
	}

	I_in <I> in () const;

	I_ptr <I> _retn () NIRVANA_NOEXCEPT
	{
		I_ptr <I> p (p_);
		p_ = 0;
		return p;
	}

	/// Destroying conversion to the ABI return value.
	/// When servant returns I_var, skeleton have to return Interface*
	/// and reset the I_var to avoid object deletion on return.
	operator Interface* () NIRVANA_NOEXCEPT
	{
		Interface* p = p_;
		p_ = 0;
		return p;
	}

protected:
	void reset (I* p) NIRVANA_NOEXCEPT
	{
		I* tmp = p_;
		p_ = p;
		interface_release (tmp);
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
void I_ptr_base <I>::move_from (I_var <I>& var) NIRVANA_NOEXCEPT
{
	p_ = var.p_;
	var.p_ = 0;
}

}
}

#endif
