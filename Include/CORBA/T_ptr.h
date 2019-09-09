#ifndef NIRVANA_ORB_T_PTR_H_
#define NIRVANA_ORB_T_PTR_H_

#include "Bridge.h"
#include <assert.h>

namespace CORBA {
namespace Nirvana {

//! Interface pointer template.
template <class I>
class T_ptr
{
	T_ptr (Bridge <I>* p) = delete;

public:
	T_ptr ()
	{} // Zero init skipped for performance

	T_ptr (I* p) :
		p_ (p)
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
		return &static_cast <Bridge <I>&> (*p_);
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

}
}

#endif
