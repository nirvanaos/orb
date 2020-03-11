#ifndef NIRVANA_CORE_COREINTERFACE_H_
#define NIRVANA_CORE_COREINTERFACE_H_

namespace Nirvana {
namespace Core {

class CoreInterface
{
public:
	virtual void core_add_ref () = 0;
	virtual void core_remove_ref () = 0;
};

template <class I>
class CoreI_var
{
public:
	CoreI_var () :
		p_ (nullptr)
	{}

	CoreI_var (I* p) :
		p_ (p)
	{}

	CoreI_var (const CoreI_var& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->core_add_ref ();
	}

	CoreI_var (CoreI_var&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~CoreI_var ()
	{
		if (p_)
			p_->core_remove_ref ();
	}

	CoreI_var& operator = (I* p)
	{
		reset (p);
		return *this;
	}

	CoreI_var& operator = (const CoreI_var& src)
	{
		I* p = src.p_;
		if (p_ != p) {
			if (p)
				p->core_add_ref ();
			reset (p)
		}
		return *this;
	}

	CoreI_var& operator = (CoreI_var&& src)
	{
		reset (src.p_);
		src.p_ = nullptr;
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
		if (p_)
			p_->core_remove_ref ();
		p_ = p;
	}

private:
	I* p_;
};

}
}

#endif
