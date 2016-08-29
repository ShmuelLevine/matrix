/**
*      @file   matrix_buffer_memory_manager.h
*      @brief  Provide allocation and deallocation functions for Matrix classes
*using hpx::serialization:serialize_buffer<T>.
*
*		       These two classes provides management of a memory pool to
*use with Matrix objects.  It is expected that the typical use case involves a
*large number of large Matrix objects required.  This class provides allocator
*and deallocator functions to support efficient creation and destruction of
*objects.
*
*              The allocator class will also abstract all details related to
*memory alignment of matrix objects to ensure maximum performance.  It is still
*incumbent on the Matrix class to ensure that appropriate padding is specified
*as part of the actual dimensions of the matrix.
*
*
*    @author   $AUTHOR$ ($AUTHORREF$), $EMAIL$
*    @version 	0.1.0
*    @date    	2015.05.12
*
*   @internal
*     Created  $DATE$
*    Revision  $Id$
*    Compiler  gcc/g++
*     Company  $COMPANY$
*   Copyright  $COPYRIGHT$
* @endinternal
*
* =============================================================================
*/

/// \addtogroup Modeling Core

#ifndef FX_CORE_MATRIX_PADDING_H
#define FX_CORE_MATRIX_PADDING_H
#include <cstdint>

namespace fx {
namespace core {
namespace matrices {

template <int64_t N> struct Padded {
public:
  static constexpr int64_t Padded_Length(int64_t spec_length) noexcept{
      return static_cast<int64_t>(((spec_length / N) + 1) * N);
  }
};

template <int64_t = 0> struct Unpadded {
  constexpr static int64_t Padded_Length(int64_t spec_length) noexcept {
    return spec_length;
  }
};
}
}
}

#endif
