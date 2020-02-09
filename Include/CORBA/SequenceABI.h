//! \file SequenceABI.h.
//!
//! \brief Declares the sequence ABI

#ifndef NIRVANA_ORB_SEQUENCEABI_H_
#define NIRVANA_ORB_SEQUENCEABI_H_

namespace CORBA {
namespace Nirvana {

template <class T>
struct SequenceABI
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
