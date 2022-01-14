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
#ifndef NIRVANA_ORB_SERVANT_REFERENCE_H_
#define NIRVANA_ORB_SERVANT_REFERENCE_H_
#pragma once

#include <utility>

namespace CORBA {

template <class T>
class servant_reference;

template <class T, class ... Args>
servant_reference <T> make_reference (Args ...);

template <class T, class ... Args>
servant_reference <T> make_stateless (Args ...);

}

namespace CORBA {

template <class T>
class servant_reference
{
public:
	servant_reference () :
		p_ (nullptr)
	{}

	servant_reference (nullptr_t) :
		p_ (nullptr)
	{}

	servant_reference (T* p) :
		p_ (p)
	{
		if (p_)
			p_->_add_ref ();
	}

	servant_reference (const servant_reference& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}

	template <class T1>
	servant_reference (servant_reference <T1>&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~servant_reference ()
	{
		release ();
	}

	servant_reference& operator = (nullptr_t)
	{
		reset (nullptr);
		return *this;
	}

	servant_reference& operator = (T* p)
	{
		reset (p);
		return *this;
	}

	servant_reference& operator = (const servant_reference& src)
	{
		reset (src.p_);
		return *this;
	}

	template <class T1>
	servant_reference& operator = (servant_reference <T1>&& src)
	{
		release ();
		p_ = src.p_;
		src.p_ = nullptr;
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

protected:
	servant_reference (T* p, bool) :
		p_ (p)
	{}

private:
	void reset (T* p)
	{
		if (p_ != p) {
			if (p)
				p->_add_ref ();
			T* tmp = p_;
			p_ = p;
			if (tmp)
				tmp->_remove_ref ();
		}
	}

	void reset (nullptr_t)
	{
		if (p_) {
			p_->_remove_ref ();
			p_ = nullptr;
		}
	}

	void release ()
	{
		if (p_)
			p_->_remove_ref ();
	}

	template <class T1, class ... Args> friend
	servant_reference <T1> make_reference (Args ... args);

	template <class T1, class ... Args> friend
	servant_reference <T1> make_stateless (Args ... args);

protected:
	T* p_;
};

template <typename T>
class servant_out
{
public:
	servant_out (servant_reference <T>& var) :
		ref_ (var)
	{
		var = nullptr;
	}

#ifdef LEGACY_CORBA_CPP
	servant_out (T*& p) :
		ref_ (reinterpret_cast <servant_reference <T>&> (p))
	{
		p = nullptr;
	}
#endif

	servant_out (const servant_out& src) :
		ref_ (src.ref_)
	{}

	servant_out& operator = (const servant_out& src)
	{
		ref_ = src.ref_;
		return *this;
	}

	servant_out& operator = (const servant_reference <T>& src)
	{
		ref_ = src;
		return *this;
	}

private:
	servant_reference <T>& ref_;
};

template <class T, class ... Args> inline
servant_reference <T> make_reference (Args ... args)
{
	return servant_reference <T> (new T (std::forward <Args> (args)...), false);
}

}

#endif
