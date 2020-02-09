#ifndef NIRVANA_ORB_SEQUENCEABI_H_
#define NIRVANA_ORB_SEQUENCEABI_H_

#include <Nirvana/NirvanaBase.h>

namespace CORBA {
namespace Nirvana {

template <class T>
struct SequenceABI
{
	void reset ()
	{
		data_.ptr = nullptr;
		data_.size = 0;
		data_.allocated = 0;
	}

	struct Data {
		T* ptr;
		size_t size;
		size_t allocated;
	} data_;
};

}
}

#endif
