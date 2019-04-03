// Nirvana project
// Sequence - CORBA sequence declarations

#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_

#include "BasicTypes.h"

namespace CORBA {

template <typename T> // For variable-length elements use var types
class sequence
{
public:
  sequence ();
  sequence (ULong max);
  sequence (ULong max, ULong length, T* data, Boolean release = FALSE);
  sequence (const sequence& src);
  ~sequence ();
  
  sequence& operator = (const sequence& src);
  
  ULong maximum () const;
  void length (ULong);
  ULong length () const;
  
  T& operator [] (ULong index);
  const T& operator [] (ULong index) const;
  
  Boolean release () const;
  
  void replace (ULong max, ULong length, T* data, Boolean release = FALSE);
  
  T* get_buffer (Boolean orphan = FALSE);
  const T* get_buffer () const;
  
private:
  T* data_;
  ULong len_;
  ULong max_;
};
  
template <typename T, ULong S>
class sequence_bounded
{
public:
  sequence_bounded ();
  sequence_bounded (ULong length, T* data, Boolean release = FALSE);
  sequence_bounded (const sequence_bounded& src);
  ~sequence_bounded ();
  
  sequence_bounded& operator = (const sequence_bounded& src);
  
  ULong maximum () const;
  void length (ULong);
  ULong length () const;
  
  T& operator [] (ULong index);
  const T& operator [] (ULong index) const;
  
  Boolean release () const;
  
  void replace (ULong length, T* data, Boolean release = FALSE);
  
  T* get_buffer (Boolean orphan = FALSE);
  const T* get_buffer () const;
  
private:
  T* data_;
  ULong len_;
};

} // namespace CORBA

#endif
