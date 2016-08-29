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

#ifndef FX_CORE_MATRIX_MEMORY_MANAGER_H
#define FX_CORE_MATRIX_MEMORY_MANAGER_H

#include <vector>
// #include <set> NOTE: use sorted vector instead of set.  See Exceptional STL
// for details

namespace fx {
namespace core {

template <typename T> class Allocation_Node { size_t actual_size };

/**
  * This class defines an allocator type which will allocate memory for a matrix
  *of a given size
      **/
template <typename T, size_t alignment = 64> class matrix_allocator {

private:
  template <typename T> using container_t = std::vector<T>;

  container_t<Allocation_Node> allocated_nodes;
};

template <typename T> class matrix_deleter {};
}
}

#endif