/**
*      @file   Matrix_Data_Allocator.hpp
*      @brief  Allocator for handling allocation/de-allocation of memory associated with Matrix_Data
*
*		       A class defining allocation and de-allocation operations for Matrix_Data operations.  
*              This particular class is intended to help reduce the burden of creating new Matrices. 
*
*
*
*    @author   $AUTHOR$ ($AUTHORREF$), $EMAIL$
*    @version 	0.1.0
*    @date    	2016.03.14
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

#ifndef FX_CORE_MATRIX_DATA_ALLOCATOR_HPP
#define FX_CORE_MATRIX_DATA_ALLOCATOR_HPP
#include <hpx/config.hpp>
#include <cstdint> // std::int64_t
//#include <memory>
#include <mkl.h> // mkl_malloc, mkl_free
#include <stack>
#include <unordered_map>
#include <hpx/include/lcos.hpp>
#include <map>
//#include <iosfwd>

/************************************************************************
 * Notes:
 * 1. TODO: consider adding support for custom allocator for the underlying
 hpx::serialization::serialize_buffer class.
 at this time, this is considered an optimization candidate and is not included
 in this code.  Update as necessary.
 ************************************************************************/

namespace fx {
    namespace core {
        namespace detail {


            class Matrix_Allocator {
            public:

                using T = float;
                using data_type = T;
                static const int64_t alignment = 64;

            private:
                using mutex_type = hpx::lcos::local::spinlock;
                using free_list_type = std::map<int64_t, std::stack<T*>>;
                using allocation_list_type = std::map<T*, int64_t>;

            public:
                Matrix_Allocator() {}
                ~Matrix_Allocator();
                Matrix_Allocator(Matrix_Allocator const&) = delete;
                Matrix_Allocator(Matrix_Allocator&&) = delete;

                static T* allocate(int64_t n);
                static void deallocate(T* p);

            private:

                static mutex_type mtx_;
                static free_list_type free_list_;
                static allocation_list_type allocation_list_;

            }; // class Matrix_Allocator
        } // namespace detail
    } // namespace core
} // namespace fx

#endif
