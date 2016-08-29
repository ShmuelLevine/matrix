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

#ifndef FX_CORE_MATRIX_ORDER_H
#define FX_CORE_MATRIX_ORDER_H

#include "Dimension_Type.hpp"
#include <cstdint>

namespace fx {
namespace core {

    struct Dimension_Type;

    template <typename M>
  struct Matrix_Storage_Order {

      using type = typename M::Order_Policy;
    };

  template <typename M>
  using Matrix_Storage_Order_t = typename Matrix_Storage_Order<M>::type;
  
namespace matrices {

struct Column_Major {
    static  int64_t Major_Dimension(fx::core::Dimension_Type const&) noexcept;
    static  int64_t Minor_Dimension(fx::core::Dimension_Type const&) noexcept;
  static int64_t Rows(fx::core::internal::Matrix_Size_Type const&) noexcept;
    static int64_t Cols(fx::core::internal::Matrix_Size_Type const&) noexcept;
};

struct Row_Major {
    static  int64_t Major_Dimension(fx::core::Dimension_Type const&) noexcept;
    static  int64_t Minor_Dimension(fx::core::Dimension_Type const&) noexcept;
    static int64_t Rows(fx::core::internal::Matrix_Size_Type const&) noexcept;
    static int64_t Cols(fx::core::internal::Matrix_Size_Type const&) noexcept;
};

}
}
}

#endif
