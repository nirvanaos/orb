#ifndef NIRVANA_CORE_COREINTERFACE_H_
#define NIRVANA_CORE_COREINTERFACE_H_

namespace Nirvana {
namespace Core {

/// Core interface.
class CoreInterface
{
public:
	virtual void core_add_ref () = 0;
	virtual void core_remove_ref () = 0;
};

/// Core interface smart pointer.
template <class I>
class CorePtr
{
public:
	CorePtr () :
		p_ (nullptr)
	{}

	/// Note that unlike I_var, CorePtr increments reference counter.
	CorePtr (I* p) :
		p_ (p)
	{
		if (p)
			p->core_add_ref ();
	}

	CorePtr (const CorePtr& src) :
		CorePtr (src.p_)
	{}

	CorePtr (CorePtr&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~CorePtr ()
	{
		if (p_)
			p_->core_remove_ref ();
	}

	/// Note that unlike I_var, CorePtr increments reference counter.
	CorePtr& operator = (I* p)
	{
		reset (p);
		return *this;
	}

	CorePtr& operator = (const CorePtr& src)
	{
		reset (src.p_);
		return *this;
	}

	CorePtr& operator = (CorePtr&& src)
	{
		if (this != &src) {
			reset (nullptr);
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	I* operator -> () const
	{
		return p_;
	}

	operator I* () const
	{
		return p_;
	}

private:
	void reset (I* p)
	{
		if (p != p_) {
			if (p)
				p->core_add_ref ();
			I* tmp = p_;
			p_ = p;
			if (tmp)
				tmp->core_remove_ref ();
		}
	}

private:
	I* p_;
};

}
}

#endif
