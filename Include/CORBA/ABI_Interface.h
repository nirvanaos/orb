#ifndef NIRVANA_ORB_ABI_INTERFACE_H_
#define NIRVANA_ORB_ABI_INTERFACE_H_

#include "Interface.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

template <class I>
struct ABI_Interface
{
	static const bool is_checked = true;

	typedef T_var <I> Var;

	typedef BridgeMarshal <I>* ABI_in;
	typedef BridgeMarshal <I>** ABI_out;
	typedef BridgeMarshal <I>** ABI_inout;
	typedef BridgeMarshal <I>* ABI_ret;

	typedef T_ptr <I> In;

	class InOut
	{
	public:
		InOut (T_var <I>& val) :
			InOut (static_cast <T_ptr <I>&> (val))
		{}

		~InOut () noexcept (false);

		BridgeMarshal <I>** operator & () const
		{
			return &ref_.p_;
		}

	protected:
		/// Using T_ptr as inout parameter is error prone.
		/// We prohibit it.
		InOut (T_ptr <I>& p) :
			ref_ (p)
		{}

	protected:
		T_ptr <I>& ref_;
	};

	class Out : public InOut
	{
	public:
		Out (T_ptr <I>& p) :
			InOut (p)
		{
			InOut::ref_ = I::_nil ();
		}

		Out (T_var <I>& val) :
			InOut (val)
		{
			release (InOut::ref_);
			InOut::ref_ = T_ptr <I>::nil ();
		}
	};

	static T_ptr <I> in (ABI_in p)
	{
		return I::unmarshal (p);
	}

	static T_var <I>& inout (ABI_inout p)
	{
		_check_pointer (p);
		I::unmarshal (*p);
		return reinterpret_cast <T_var <I>&> (**p);
	}

	static T_var <I>& out (ABI_out p)
	{
		_check_pointer (p);
		if (*p)
			::Nirvana::throw_BAD_PARAM ();
		return reinterpret_cast <T_var <I>&> (**p);
	}

	void check_or_clear (T_ptr <I>& p);
};

// Outline for compact code
template <class I>
void ABI_Interface <I>::check_or_clear (T_ptr <I>& p)
{
	try {
		I::unmarshal (p);
	} catch (...) {
		release (p);
		p = T_ptr <I>::nil ();
		throw;
	}
}

// Outline for compact code
template <class I>
ABI_Interface <I>::InOut::~InOut () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		check_or_clear (ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

template <class I>
struct ABI <I, typename std::enable_if <std::is_base_of <Bridge <Interface>, I>::value, void>::type> :
	public ABI_Interface <I>
{};

}
}

#endif
