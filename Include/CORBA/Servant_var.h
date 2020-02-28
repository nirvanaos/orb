#ifndef NIRVANA_ORB_SERVANTVAR_H_
#define NIRVANA_ORB_SERVANTVAR_H_

namespace PortableServer {

template <typename Servant> class Servant_out;

template <typename Servant>
class Servant_var
{
protected:
	void swap (Servant* lhs, Servant* rhs)
	{
		Servant* tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

public:
	Servant_var () :
		p_ (0)
	{}

	Servant_var (Servant* p) :
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
		src.p_ = 0;
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

	Servant_var& operator = (Servant* p)
	{
		if (p_ != p)
			reset (p);
		return *this;
	}

	Servant_var& operator = (const Servant_var& src)
	{
		if (p_ != src.p_) {
			Servant* p = src.p_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

	Servant_var& operator = (Servant_var&& src)
	{
		if (p_ != src.p_) {
			reset (src.p_);
			src.p_ = 0;
		}
		return *this;
	}

	Servant* operator -> () const
	{
		if (!p_)
			::Nirvana::throw_INV_OBJREF ();
		return p_;
	}

	operator Servant* () const
	{
		return p_;
	}

	Servant* in () const
	{
		return p_;
	}

	Servant_var <Servant>& inout ()
	{
		return *this;
	}

	Servant_out <Servant> out ();

	Servant* _retn ()
	{
		Servant* retval = p_;
		p_ = 0;
		return retval;
	}

private:
	void reset (Servant* p) NIRVANA_NOEXCEPT
	{
		Servant* tmp = p_;
		p_ = p;
		if (tmp)
			tmp->_remove_ref ();
	}

	friend class Servant_out <Servant>;

private:
	Servant* p_;
};

template <typename Servant>
class Servant_out
{
public:
	Servant_out (Servant*& p) :
		ref_ (p)
	{
		p = 0;
	}

	Servant_out (Servant_var <Servant>& var) :
		ref_ (var.p_)
	{
		var = (Servant*)0;
	}

	Servant_out (const Servant_out& src) :
		ref_ (src.ref_)
	{}

	Servant_out& operator = (const Servant_out& src)
	{
		if (&ref_ != &src.ref_) {
			Servant* p = src.ref_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

	Servant_out& operator = (Servant* p)
	{
		reset (p);
		return *this;
	}

	Servant_out& operator = (const Servant_var <Servant>& src)
	{
		if (&ref_ != &src.p_) {
			Servant* p = src.p_;
			if (p)
				p->_add_ref ();
			reset (p);
		}
		return *this;
	}

private:
	void reset (Servant* p) NIRVANA_NOEXCEPT
	{
		Servant* tmp = ref_;
		ref_ = p;
		if (tmp)
			tmp->_remove_ref ();
	}

private:
	Servant*& ref_;
};

template <class Servant> inline
Servant_out <Servant> Servant_var <Servant>::out ()
{
	return Servant_out <Servant> (*this);
}

}

#endif
