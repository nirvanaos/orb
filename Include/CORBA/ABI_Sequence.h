//! \file ABI_Sequence.h.
//!
//! \brief Declares the sequence ABI

#ifndef NIRVANA_ORB_ABI_SEQUENCE_H_
#define NIRVANA_ORB_ABI_SEQUENCE_H_

namespace std {
template <class T, class A> class vector;
template <class T> class allocator;
}

namespace CORBA {
namespace Nirvana {

template <class T>
using Sequence = std::vector <T, std::allocator <T> >;

template <class T>
struct alignas (sizeof (void*)) ABI <Sequence <T> >
{
	void reset ()
	{
		ptr = nullptr;
		size = 0;
		allocated = 0;
	}

	T* ptr;
	size_t size;
	size_t allocated;
};

}
}

#endif
