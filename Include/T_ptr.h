#ifndef NIRVANA_ORB_T_PTR_H_
#define NIRVANA_ORB_T_PTR_H_

namespace CORBA {
namespace Nirvana {

//! General T_ptr template
template <class T> class T_ptr;

//! General T_var template
template <class T> class T_var;

//! General T_out template
template <class T> class T_out;

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
		p_ (static_cast <T*> (p))
	{}

	template <class T1>
	T_ptr (T_ptr <T1> src) :
		p_ (&static_cast <T&> (*src.p_))
	{}

	template <class T1>
	T_ptr& operator = (T_ptr <T1> src)
	{
		p_ = &static_cast <T&> (*src.p_);
		return *this;
	}

	operator Bridge <T>* () const
	{
		return static_cast <Bridge <T>*> (p_);
	}

	T* operator -> () const
	{
		return p_;
	}

	operator bool () const
	{
		return p_ != 0;
	}

	static T_ptr nil ()
	{
		return T_ptr ((T*)0);
	}

private:
	template <class T1> friend class T_ptr;

private:
	T* p_;
};

/// All interfaces derived from Interface class.
/// This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;
typedef ::CORBA::Nirvana::T_ptr <Interface> Interface_ptr;

}

inline bool is_nil (Nirvana::Interface_ptr itf);
inline void release (Nirvana::Interface_ptr itf);

namespace Nirvana {

// T_var helper class

template <class T>
class T_var : public T_ptr <T>
{
public:
	T_var () :
		T_ptr <T> ((T*)nullptr)
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
		return ptr_;
	}

	const T* operator -> () const
	{
		return ptr_;
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
		: ptr_ (p)
	{
		ptr_ = 0;
	}

	T_out (T_var <T>& p)
		: ptr_ (p.out ())
	{}

	T_out (const T_out& p)
		: ptr_ (p.ptr_)
	{}

	T_out <T>& operator = (T_out <T>& p)
	{
		ptr_ = p.ptr_;
		return *this;
	}

	T_out <T>& operator = (T_ptr <T> p)
	{
		ptr_ = p;
		return *this;
	}

	operator T*& ()
	{
		return ptr_;
	}

	T*& ptr ()
	{
		return ptr_;
	}

	T* operator -> ()
	{
		return ptr_;
	}

private:
	T_ptr <T>& ptr_;
	// assignment from T_var is not allowed
	void operator = (const T_var <T> &);
};

}
}

#endif
