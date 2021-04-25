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
#ifndef NIRVANA_ORB_SERVANTVAR_H_
#define NIRVANA_ORB_SERVANTVAR_H_

#ifdef LEGACY_CORBA_CPP

namespace PortableServer {

template <typename T> class Servant_out;

template <typename T>
class Servant_var
{
public:
	Servant_var () :
		p_ (nullptr)
	{}

	Servant_var (T* p) :
		p_ (p)
	{}

	Servant_var (const Servant_var& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}

	Servant_var (Servant_var&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~Servant_var ()
	{
		if (p_) {
			try {
				p_->_remove_ref ();
			} catch (...) {
				// swallow exceptions
			}
		}
	}

	Servant_var& operator = (T* p)
	{
		if (p_ != p)
			reset (p);
		return *this;
	}

	Servant_var& operator = (const Servant_var& src)
	{
		if (p_ != src.p_) {
			T* p = src.p_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

	template <class T1>
	Servant_var& operator = (Servant_var <T1>&& src)
	{
		if (p_ != src.p_) {
			reset (src.p_);
			src.p_ = nullptr;
		}
		return *this;
	}

	T* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	operator T* () const
	{
		return p_;
	}

	T* in () const
	{
		return p_;
	}

	Servant_var <T>& inout ()
	{
		return *this;
	}

	Servant_out <T> out ();

	T* _retn ()
	{
		T* retval = p_;
		p_ = nullptr;
		return retval;
	}

private:
	void reset (T* p) NIRVANA_NOEXCEPT
	{
		T* tmp = p_;
		p_ = p;
		if (tmp)
			tmp->_remove_ref ();
	}

	friend class Servant_out <T>;

private:
	T* p_;
};

template <typename T>
class Servant_out
{
public:
	Servant_out (T*& p) :
		ref_ (p)
	{
		p = nullptr;
	}

	Servant_out (Servant_var <T>& var) :
		ref_ (var.p_)
	{
		var = nullptr;
	}

	Servant_out (const Servant_out& src) :
		ref_ (src.ref_)
	{}

	Servant_out& operator = (const Servant_out& src)
	{
		if (&ref_ != &src.ref_) {
			T* p = src.ref_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

	Servant_out& operator = (T* p)
	{
		reset (p);
		return *this;
	}

	Servant_out& operator = (const Servant_var <T>& src)
	{
		if (&ref_ != &src.p_) {
			T* p = src.p_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

private:
	void reset (T* p) NIRVANA_NOEXCEPT
	{
		T* tmp = ref_;
		ref_ = p;
		if (tmp)
			tmp->_remove_ref ();
	}

private:
	T*& ref_;
};

template <class T> inline
Servant_out <T> Servant_var <T>::out ()
{
	return Servant_out <T> (*this);
}

}

#endif

#endif
