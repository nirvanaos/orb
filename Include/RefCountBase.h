#ifndef NIRVANA_ORB_REFCOUNTBASE_H_
#define NIRVANA_ORB_REFCOUNTBASE_H_

#include "../../core/Source/AtomicCounter.h"

namespace CORBA {
namespace Nirvana {

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
			final_release ();
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

	virtual void final_release ()
	{
		delete this;
	}

private:
	::Nirvana::Core::RefCounter ref_count_;
};

}
}

#undef USE_INTRINSIC_ATOMIC

#endif
