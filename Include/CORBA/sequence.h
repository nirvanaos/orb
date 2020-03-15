// Nirvana project
// Sequence - CORBA sequence declarations

#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_

#include <Nirvana/vector.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Nirvana {

template <class T>
struct Type <Sequence <T> > :
	public TypeVarLen <Sequence <T>, CHECK_SEQUENCES || Type <T>::has_check>
{
	typedef TypeVarLen <Sequence <T>, CHECK_SEQUENCES || Type <T>::has_check> Base;
	typedef typename Type <T>::ABI_type T_ABI;
	typedef ABI <Sequence <T_ABI> > ABI_type;

	static void check (const ABI_type& v);

	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;

	class C_in : public Base::C_in
	{
	public:
		C_in (const Sequence <T>& v) :
			Base::C_in (v)
		{}

		const ABI_type* operator & () const
		{
			return &static_cast <const ABI_type&> (this->ref_);
		}
	};

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Sequence <T>& s) :
			Base::C_inout (s)
		{}

		ABI_type* operator & () const
		{
			return &static_cast <ABI_type&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (Sequence <T>& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static const Sequence <T>& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <const Sequence <T>&> (*p);
	}

	static Sequence <T>& inout (ABI_inout p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <Sequence <T>&> (*p);
	}

	static Sequence <T>& out (ABI_out p)
	{
		Base::out (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		Sequence <T>& val = static_cast <Sequence <T>&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}
};

template <class T>
void Type <Sequence <T> >::check (const ABI_type& v)
{
	// Do some check
	if (CHECK_SEQUENCES) {
		const T* p = v.ptr;
		if (p)
			CORBA::Nirvana::_check_pointer (p);
		size_t cnt = v.size;
		if (cnt > 0 && (cnt > v.allocated / sizeof (T) || !Sequence <T>::memory ()->is_readable (p, cnt * sizeof (T))))
			::Nirvana::throw_BAD_PARAM ();
	}

	if (Type <T>::has_check) {
		for (const T_ABI* p = v.ptr, *end = p + v.size; p != end; ++p)
			Type <T>::check (*p);
	}
}

template <typename T>
using TypeSequence = Type <Sequence <T> >;

template <typename T>
using Sequence_in = typename TypeSequence <T>::C_in;

template <typename T>
using Sequence_out = typename TypeSequence <T>::C_out;

template <typename T>
using Sequence_inout = typename TypeSequence <T>::C_inout;

template <typename T>
using Sequence_var = typename TypeSequence <T>::C_var;

}
} // namespace CORBA

#endif
