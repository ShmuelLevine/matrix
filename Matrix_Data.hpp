/**
*      @file   Matrix_Data.hpp
*      @brief  Class implementing memory management for the Matrix contents
*
*		       A separate class for managing the memory associated with a Matrix.
*              This allows for the simplification of the Matrix class, and it also
*              separates the Matrix class into separate class, each with a single
*              responsibility.
*
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

#ifndef FX_CORE_MATRIX_DATA_HPP
#define FX_CORE_MATRIX_DATA_HPP

//#include "stdafx.h"
//#include "export_definitions.hpp"
#include "Matrix_Copy_Policy.hpp"
#include "Matrix_Data_Allocator.hpp"
#include <hpx/include/serialization.hpp>

#include <memory>
#include <algorithm>
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

class Matrix_Data {
public:

  using T = float;
  using data_type = T;
  

private:

  using alloc_type = fx::core::detail::Matrix_Allocator;
  using buffer_type = hpx::serialization::serialize_buffer<
  float>;
 
  static alloc_type alloc_;
  int64_t data_buffer_elements_;
  buffer_type data_;

  struct hold_reference
  {
      hold_reference(buffer_type const& data)
          : data_(data)
      {}

      void operator()(data_type*) {}     // no deletion necessary

      buffer_type data_;
  };


public:


  Matrix_Data();
  Matrix_Data(int64_t elements);
Matrix_Data(Matrix_Data const &src, shallow_copy_);
Matrix_Data(Matrix_Data const &src, deep_copy_);
Matrix_Data(Matrix_Data const& src) : Matrix_Data(src, deep_copy) {}
Matrix_Data(Matrix_Data && src);
  ~Matrix_Data() {} 
  friend void swap(Matrix_Data &a, Matrix_Data &b);

  bool operator==(Matrix_Data const &other);
  bool operator==(Matrix_Data const &other) const;
  bool operator!=(Matrix_Data const &other);
  bool operator!=(Matrix_Data const &other) const;

  Matrix_Data &operator=(Matrix_Data const &other);
  Matrix_Data &operator=(Matrix_Data &&other);

  buffer_type::value_type* Data() {return data_.data();}
 const  buffer_type::value_type * Data() const  {return data_.data();}
   
   T &operator[](int64_t index) { return data_[index]; }
   T operator[](int64_t index) const {
    return T{data_[index]}; // Explictly construct a T (likely from a reference)
  }
   static void deallocate(T* ptr) { alloc_.deallocate(ptr); }
  

private:
  // Serialization support: even if all of the code below runs on one
  // locality only, we need to provide an (empty) implementation for the
  // serialization as all arguments passed to actions have to support this.
  friend class hpx::serialization::access;
  template <typename Archive>
   void serialize(Archive &ar, const unsigned int version) {
      ar & data_buffer_elements_ &data_;
    }

}; // class Matrix_Data


void swap(Matrix_Data &a, Matrix_Data  &b);
} // namespace detail
} // namespace core
} // namespace fx 

#endif
