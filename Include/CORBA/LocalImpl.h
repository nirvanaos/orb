/// Locally implemented pseudo objects (Environment etc)
#ifndef NIRVANA_ORB_LOCALIMPL_H_
#define NIRVANA_ORB_LOCALIMPL_H_

namespace CORBA {
namespace Nirvana {

template <class T> class Local_inout;

template <class T>
class Local_var
{
public:
	Local_var () :
		p_ (0)
	{}

	Local_var (T* p) :
		p_ (p)
	{}

	Local_var (const Local_var& src) :
		p_ (T::_duplicate (src.p_))
	{}

	Local_var& operator = (T* p)
	{
		reset (p);
		return *this;
	}

	Local_var& operator = (const Local_var& src)
	{
		if (this != &src)
			reset (T::_duplicate (src.p_));
		return *this;
	}

	Local_var& operator = (Local_var&& src)
	{
		if (this != &src) {
			reset (src.p_);
			src.p_ = 0;
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

	operator bool () const
	{
		return p_ != 0;
	}

	T* _retn ()
	{
		T* p = p_;
		p_ = 0;
		return p;
	}

private:
	void reset (T* p) NIRVANA_NOEXCEPT
	{
		release (p_);
		p_ = p;
	}

private:
	friend class Local_inout <T>;

	T* p_;
};

template <class T>
class Local_inout
{
public:
	Local_inout (Local_var <T>& var) :
		ref_ (var.p_)
	{}

protected:
	Local_inout (T* p) :
		ref_ (p)
	{}

protected:
	T*& ref_;
};

}
}

#endif
