#ifndef NIRVANA_ORB_T_PTR_H_
#define NIRVANA_ORB_T_PTR_H_

#include <cassert>

namespace CORBA {

namespace Nirvana {

//! General T_ptr template
template <class T> class T_ptr;

//! General T_var template
template <class T> class T_var;

//! General T_out template
template <class T> class T_out;

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

protected:
	T_ptr (T* p) :
		m_p (p)
	{}

	operator T* () const
	{
		return m_p;
	}

private:
	template <class T1> friend class T_ptr;
	friend class ClientInterface <T>;
	template <class S, class I> friend class Skeleton;

  friend void CORBA::release <T> (T_ptr <T>);
	friend bool CORBA::is_nil <T> (T_ptr <T>);

private:
	T* m_p;
};

// T_var helper class

template <class T>
class T_var : public T_ptr <T>
{
public:
	T_var () :
		T_ptr <T> (0)
	{}

	T_var (T_ptr <T> p) :
		T_ptr <T> (p)
	{}

	T_var (const T_var <T>& src) :
		T_ptr <T> (T::_duplicate (src))
	{}

	~T_var ()
	{
		release (*this);
	}

	T_var <T>& operator = (T_ptr <T> p)
	{
		release (*this);
		T_ptr <T>::operator = (p);
		return *this;
	}

	T_var <T>& operator = (const T_var <T>& src)
	{
		if (src != this) {
			release (*this);
			T_ptr <T>::operator = (T::_duplicate (src));
		}
		return *this;
	}
/*
	T* operator -> ()
	{
		return m_ptr;
	}

	const T* operator -> () const
	{
		return m_ptr;
	}
*/
	T_ptr <T> in () const
	{
		return *this;
	}

	T_ptr <T>& inout ()
	{
		return *this;
	}

	T_ptr <T>& out ()
	{
		operator = (T_ptr <T>::nil ());
		return *this;
	}

	T* _retn ()
	{
		T* p = *this;
		T_ptr <T>::operator = (T_ptr <T>::nil ());
		return p;
	}
};

// T_out helper class

template <class T>
class T_out
{
public:
	T_out (T_ptr <T>& p)
		: m_ptr (p)
	{
		m_ptr = 0;
	}

	T_out (T_var <T>& p)
		: m_ptr (p.out ())
	{}

	T_out (const T_out& p)
		: m_ptr (p.m_ptr)
	{}

	T_out <T>& operator = (T_out <T>& p)
	{
		m_ptr = p.m_ptr;
		return *this;
	}

	T_out <T>& operator = (T_ptr <T> p)
	{
		m_ptr = p;
		return *this;
	}

	operator T*& ()
	{
		return m_ptr;
	}

	T*& ptr ()
	{
		return m_ptr;
	}

	T* operator -> ()
	{
		return m_ptr;
	}

private:
	T_ptr <T>& m_ptr;
	// assignment from T_var is not allowed
	void operator = (const T_var <T> &);
};

}
}

#endif
