#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_

#include "Environment_c.h"
#include "ServantImpl.h"
#include "Environment_s.h"
#include "SystemException.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBase :
	public Bridge < ::CORBA::Environment>
{
public:
	void exception_set (Long code, const char* rep_id, const void* param,
		const ExceptionEntry* user_exceptions = nullptr);

	const Char* exception_id () const;

	const void* exception_value () const;

	void exception_free ();

	Exception* exception () const
	{
		if (data_.is_small)
			return (Exception*)(data_.small);
		else
			return data_.ptr;
	}

	void exception (Exception* ex);
	void set (const Exception* ex);

	void check () const;

	EnvironmentBase& operator = (const EnvironmentBase& src)
	{
		set (src.exception ());
		return *this;
	}

	EnvironmentBase& operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		exception_free ();
		data_ = src.data_;
		src.data_.reset ();
		return *this;
	}

protected:
	EnvironmentBase (const EPV& epv) :
		Bridge < ::CORBA::Environment> (epv)
	{
		data_.reset ();
	}

	~EnvironmentBase ()
	{
		if (data_.is_small || data_.ptr)
			exception_free ();
	}

private:
	void set (TypeCode_ptr tc, const void* data);

private:
	union Data
	{
		int small [(sizeof (SystemException) + sizeof (int) - 1) / sizeof (int)];
		struct
		{
			uintptr_t is_small;
			Exception* ptr;
		};

		void reset ()
		{
			is_small = 0;
			ptr = nullptr;
		}
	} data_;
};

}
}

#endif
