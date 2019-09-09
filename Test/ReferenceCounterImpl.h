#ifndef NIRVANA_TESTORB_REFERENCECOUNTERIMPL_H_
#define NIRVANA_TESTORB_REFERENCECOUNTERIMPL_H_

#include <CORBA/ReferenceCounter_s.h>
#include <CORBA/DynamicServant_s.h>
#include <CORBA/Implementation.h>

namespace CORBA {
namespace Nirvana {

class ReferenceCounterBase
{
public:
	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		if (!--ref_cnt_ && dynamic_) {
			try {
				dynamic_->_delete ();
			} catch (...) {
				assert (false); // Swallow exception or log
			}
		}
	}

	ULong _refcount_value () const
	{
		return ref_cnt_;
	}

protected:
	ReferenceCounterBase (DynamicServant_ptr dynamic) :
		ref_cnt_ (1),
		dynamic_ (dynamic)
	{}

private:
	ULong ref_cnt_;
	DynamicServant_ptr dynamic_;
};

class ReferenceCounterCore :
	public ImplementationPseudo <ReferenceCounterCore, ReferenceCounter>,
	public LifeCycleNoCopy <ReferenceCounterCore>,
	public ReferenceCounterBase
{
public:
	ReferenceCounterCore (DynamicServant_ptr dynamic) :
		ReferenceCounterBase (dynamic)
	{}
};
}
}

#endif
