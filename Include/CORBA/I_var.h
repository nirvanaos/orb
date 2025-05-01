/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_I_VAR_H_
#define NIRVANA_ORB_I_VAR_H_
#pragma once

namespace CORBA {
namespace Internal {

template <class Itf> class I_out;

/// Helper class to support legacy interface smart pointer.
template <class Itf>
class I_var : public I_ref <Itf>
{
	typedef I_ref <Itf> Base;

public:
	I_var () noexcept
	{}

	// No add reference on construct

	I_var (Itf* p) noexcept :
		Base (p)
	{}

	I_var (BridgeVal <Itf>* p) :
		Base (static_cast <Itf*> (static_cast <Bridge <Itf>*> (p)))
	{}

	template <class VB, typename T>
	I_var (ValueBox <VB, T>* p) noexcept :
		Base (static_cast <Itf*> (static_cast <Bridge <Itf>*> (p)))
	{}

	I_var (const I_ptr <Itf>& p) noexcept :
		Base (p.p_)
	{}

	template <class I1>
	I_var (const I_ptr <I1>& p) :
		Base (Base::wide (p.p_))
	{}

	I_var (const I_ref <Itf>& src) :
		Base (src)
	{}

	template <class I1>
	I_var (const I_ref <I1>& src) = delete;

	I_var (const I_var& src) :
		Base (src)
	{}

	template <class I1>
	I_var (const I_var <I1>& src) = delete;

	I_var (I_ref <Itf>&& src) noexcept :
		Base (std::move (src))
	{}

	template <class I1>
	I_var (const I_ref <I1>&& src) = delete;

	I_var (I_var&& src) noexcept :
		Base (std::move (src))
	{}

	template <class I1>
	I_var (const I_var <I1>&& src) = delete;

	// No add reference
	I_var& operator = (Itf* p) noexcept
	{
		// Even if p == p_ we must release p_.
		// So we don't compare pointers.
		Itf* tmp = Base::p_;
		Base::p_ = p;
		interface_release (tmp);
		return *this;
	}

	// No add reference
	I_var& operator = (const I_ptr <Itf>& p) noexcept
	{
		return operator = (p.p_);
	}

	// No add reference
	template <class I1>
	I_var& operator = (const I_ptr <I1>& p)
	{
		return operator = (Base::wide (p.p_));
	}

	// No add reference
	I_var& operator = (BridgeVal <Itf>* p) noexcept
	{
		return operator = (Base (static_cast <Itf*> (static_cast <Bridge <Itf>*> (p))));
	}

	// No add reference
	template <class VB, typename T>
	I_var& operator = (ValueBox <VB, T>* p) noexcept
	{
		return operator = (Base (static_cast <Itf*> (static_cast <Bridge <Itf>*> (p))));
	}

	// Add reference
	I_var& operator = (const I_var& src)
	{
		Base::operator = (src);
		return *this;
	}

	I_var& operator = (I_var&& src) noexcept
	{
		Base::operator = (std::move (src));
		return *this;
	}

	I_var& operator = (std::nullptr_t) noexcept
	{
		Base::operator = (nullptr);
		return *this;
	}

	inline I_inout <Itf> inout ();
	inline I_out <Itf> out ();

	I_ptr <Itf> in () const
	{
		return Base::p_;
	}

	I_ptr <Itf> _retn () noexcept
	{
		I_ptr <Itf> p (Base::p_);
		Base::p_ = nullptr;
		return p;
	}
};

}
}

#endif
