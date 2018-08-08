#ifndef NIRVANA_ORB_REFCOUNTBASE_H_
#define NIRVANA_ORB_REFCOUNTBASE_H_

#include <InterlockedCounter.h>
#include <atomic>

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
			delete this;
	}

	ULong _refcount_value () const
	{
		return ref_count_;
	}

protected:
	RefCountBase () :
		ref_count_ (1)
	{}

	virtual ~RefCountBase ()
	{}

private:
	::Nirvana::InterlockedCounter ref_count_;
};

}
}

#endif
