#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_

#include "ServantImpl.h"
#include "Environment_s.h"
#include "SystemException.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBase :
	public Bridge < ::CORBA::Environment>
{
public:
	void exception_set (Long code, String_in rep_id, const void* param,
		const ExceptionEntry* user_exceptions = 0) NIRVANA_NOEXCEPT;

	const Char* exception_id () const NIRVANA_NOEXCEPT;

	const void* exception_value () const NIRVANA_NOEXCEPT;

	void exception_free () NIRVANA_NOEXCEPT;

	void clear () NIRVANA_NOEXCEPT
	{
		exception_free ();
	}

	Exception* exception () const NIRVANA_NOEXCEPT
	{
		if (data_.is_small)
			return (Exception*)(&data_);
		else
			return data_.ptr;
	}

	void exception (Exception* ex) NIRVANA_NOEXCEPT;

	void check () const;

protected:
	EnvironmentBase (const EPV& epv) NIRVANA_NOEXCEPT :
		Bridge < ::CORBA::Environment> (epv)
	{
		data_.reset ();
	}

	~EnvironmentBase () NIRVANA_NOEXCEPT
	{
		if (data_.is_small || data_.ptr)
			exception_free ();
	}

	void move_from (EnvironmentBase& src) NIRVANA_NOEXCEPT;

private:
	bool set (const ExceptionEntry& ee) NIRVANA_NOEXCEPT;
	void set_system (const ExceptionEntry& ee, const void* data) NIRVANA_NOEXCEPT;
	void set_user (const ExceptionEntry& ee, const void* data) NIRVANA_NOEXCEPT;
	bool set_user (String_in rep_id, const void* param, const ExceptionEntry* user_exceptions) NIRVANA_NOEXCEPT;

private:
	union Data
	{
		int small [(sizeof (SystemException) + sizeof (int) - 1) / sizeof (int)];
		struct
		{
			// If small data contains Exception object, the first word of it
			// contains pointer to the virtual table and can't be zero.
			uintptr_t is_small;
			Exception* ptr;
			size_t size;
		};

		void reset ()
		{
			is_small = 0;
			ptr = 0;
		}
	} data_;
};

template <class S>
class EnvironmentImpl :
	public EnvironmentBase,
	public Skeleton <S, ::CORBA::Environment>,
	public ServantTraits <S>
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, EnvironmentBridge* env)
	{
		set_NO_IMPLEMENT (env);
		return 0;
	}

	template <class I>
	static void __release (Interface* itf)
	{}

protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

}
}

#endif
