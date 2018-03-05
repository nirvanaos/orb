#pragma once
#include <cassert>

namespace CORBA {

namespace Nirvana {

//! General T_ptr template
template <class I> class T_ptr;

}

class AbstractBase;

template <class I>
void release (Nirvana::T_ptr <I>);

template <class I>
bool is_nil (Nirvana::T_ptr <I>);

namespace Nirvana {

//! Client part of an interface 
template <class I> class ClientInterface;

//! Intermediate part of an interface
template <class I> class Bridge;

//! General T_ptr template
template <class T>
class T_ptr
{
public:
	T_ptr ()
		// Zeroinit skipped for performance
	{}

	T_ptr (Bridge <T>* p) :
		m_p (static_cast <T*> (p))
	{}

	template <class T1>
	T_ptr (T_ptr <T1> src) :
		m_p (&static_cast <T&> (static_cast <Bridge <T>&> (*src.m_p)))
	{}

	template <class T1>
	T_ptr& operator = (T_ptr <T1> src)
	{
		m_p = &static_cast <T&> (static_cast <Bridge <T>&> (*src.m_p));
		return *this;
	}

	T* operator -> () const
	{
		return m_p;
	}

	operator bool () const
	{
		return m_p != 0;
	}

	static T_ptr nil ()
	{
		return T_ptr ((T*)0);
	}

private:
	template <class T1>
	friend class T_ptr;
	
	friend class ClientInterface <T>;

  friend void CORBA::release <T> (T_ptr <T>);
	friend bool CORBA::is_nil <T> (T_ptr <T>);

	T_ptr (T* p) :
		m_p (p)
	{}

	operator T* () const
	{
		return m_p;
	}

private:
	T* m_p;
};

}
}
