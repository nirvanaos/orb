#ifndef NIRVANA_ORB_T_PTR_H_
#define NIRVANA_ORB_T_PTR_H_

#include <Nirvana/throw_exception.h>
#include "Bridge.h"

namespace CORBA {
namespace Nirvana {

template <class I> class I_inout;
template <class I> class I_var;

	//! Interface pointer template.
template <class I>
class I_ptr
{
	I_ptr (Bridge <I>* p) = delete;

public:
	constexpr I_ptr ()
	{} // Zero init skipped for performance

	constexpr I_ptr (I* p) :
		p_ (p)
	{}

	I_ptr (const I_ptr <I>& src) :
		p_ (src.p_)
	{}

	template <class I1>
	I_ptr (const I_ptr <I1>& src, bool check_nil = true)
	{
		if (src.p_) {
			*this = src.p_->operator I_ptr ();
			if (check_nil && !p_)
				::Nirvana::throw_INV_OBJREF ();
		} else
			p_ = nullptr;
	}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	inline I_ptr (I_var <I>&& var);

	operator Bridge <I>* () const
	{
		return &static_cast <Bridge <I>&> (*p_);
	}

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

	static I_ptr <I> nil ()
	{
		return I_ptr ((I*)nullptr);
	}

private:
	template <class I1> friend class I_ptr;
	friend class I_inout <I>;

private:
	I* p_;
};

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
		return static_cast <I*> (itf);
	}
};

}
}

#endif