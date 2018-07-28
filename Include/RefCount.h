#ifndef NIRVANA_ORB_REFCOUNT_H_
#define NIRVANA_ORB_REFCOUNT_H_

#include "BasicTypes.h"
#include <atomic>

namespace CORBA {
namespace Nirvana {

class RefCount
{
public:
	RefCount () :
		refcnt_ (1)
	{}

	operator ULong () const
	{
		return refcnt_;
	}

	void increment ()
	{
		++refcnt_;
	}

	ULong decrement ()
	{
		return --refcnt_;
	}

private:
	std::atomic <ULong> refcnt_;
};

class RefCountBase
{
	// Copy prohibited.
	RefCountBase (const RefCountBase&);
	RefCountBase& operator = (const RefCountBase&);

public:
	void _add_ref ()
	{
		ref_count_.increment ();
	}

	void _remove_ref ()
	{
		if (!ref_count_.decrement ())
			delete this;
	}

	ULong _refcount_value () const
	{
		return ref_count_;
	}

protected:
	RefCountBase ()
	{}

	virtual ~RefCountBase ()
	{}

private:
	RefCount ref_count_;
};

}
}

#endif
