#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_

#include "I_ptr.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_in;
template <class I> class I_out;
template <class I> class I_inout;

//! I_var helper class for interface
template <class I>
class I_var : public I_ptr <I>
{
public:
	I_var () :
		I_ptr <I> (I::_nil ())
	{}

	I_var (I_ptr <I> p) :
		I_ptr <I> (p)
	{}

	I_var (const I_var <I>& src) :
		I_ptr <I> (I::_duplicate (src))
	{}

	I_var (I_var <I>&& src) NIRVANA_NOEXCEPT :
		I_ptr <I> (src)
	{
		static_cast <I_ptr <I>&> (src) = I::_nil ();
	}

	~I_var ()
	{
		release (*this);
	}

	I_var <I>& operator = (I_ptr <I> p)
	{
		reset (p);
		return *this;
	}

	I_var <I>& operator = (const I_var <I>& src)
	{
		if (&src != this) {
			reset (I::_nil ());
			I_ptr <I>::operator = (I::_duplicate (src));
		}
		return *this;
	}

	I_var <I>& operator = (I_var <I>&& src) NIRVANA_NOEXCEPT
	{
		if (&src != this) {
			reset (src);
			static_cast <I_ptr <I>&> (src) = I::_nil ();
		}
		return *this;
	}

	inline I_in <I> in () const;
	inline I_inout <I> inout ();
	inline I_out <I> out ();

	I_ptr <I> _retn ()
	{
		I_ptr <I> p = *this;
		I_ptr <I>::operator = (I_ptr <I>::nil ());
		return p;
	}

	/// Destroying conversion to the ABI return value.
	/// When servant returns I_var, skeleton have to return Interface*
	/// and reset the I_var to avoid object deletion on return.
	operator Interface* ()
	{
		Interface* p = I_ptr <I>::operator Bridge <I> * ();
		I_ptr <I>::operator = (I_ptr <I>::nil ());
		return p;
	}

protected:
	void reset (I_ptr <I> p)
	{
		release (*this);
		I_ptr <I>::operator = (p);
	}
};

}
}

#endif
