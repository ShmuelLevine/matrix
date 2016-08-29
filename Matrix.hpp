/**
*      @file   Matrix.hpp
*      @brief  Primary template for basic serializable matrix class
*
*		       Matrix is an object designed to be serializable and used
*with the HPX runtime system.  At this time, matrix is dynamically-sized and
*allows the use of a custom memory allocation object, since most use cases are
*expected to utilize large quantities of matrices of similar size.
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

#ifndef FX_CORE_Matrix_H
#define FX_CORE_Matrix_H
/*

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
*/
//#include "stdafx.h"
#include "Dimension_Type.hpp"
#include <cstddef>
#include "export_definitions.hpp"
#include "Matrix_Padding_Policy.hpp"
#include "Matrix_Order_Policy.hpp"
//#include <hpx/include/serialization.hpp>
#include "Matrix_Copy_Policy.hpp"
#include <vector>
#include "Matrix_Data.hpp"
//#include <hpx/util/serialize_buffer.hpp>
//#include <hpx/hpx_fwd.hpp>
//#include <boost/serialization/access.hpp>
//#include "util/Exceptions.hpp"
#include <memory>
#include <algorithm>
#include <iosfwd>

/************************************************************************
 * Notes:
 * 1. TODO: consider adding support for custom allocator for the underlying
 hpx::serialization::serialize_buffer class.
 at this time, this is considered an optimization candidate and is not included
 in this code.  Update as necessary.
 ************************************************************************/

namespace hpx{ namespace serialization { class access; } }

namespace fx {
namespace core {
  // namespace detail {
  //   class Matrix_Data;
  // }


// At this time, for (compiler) performance and possibly other reasons as well,
// the matrix class will not be a template class.
// Instead, it will be based on:
// 1. Column Major
// 2. Leading dimension padded to multiple of 64 elements
// template < typename T, typename Padding_Policy = matrices::Padded<64>,
// typename Order_Policy = matrices::Column_Major >
struct Matrix_Details;

class Matrix {
public:
  class matrix_iterator;

  using dimension_type = fx::core::Dimension_Type;

  /* struct Details { ... };  */
#include "Matrix.Details.hpp"
  
  /*
   * These aliases replace the template type parameters for this class.  If/when
   * it becomes desirable to re-introduce
   * template parameters to this class, it should be possible to delete these
   * aliases and replace with template parameters
   * with default arguments, as per the original design concept
   */
  using T = float;
  using data_type = T;
  using Padding_Policy = matrices::Padded<64>;
  using Order_Policy = matrices::Column_Major;

private:
  using buffer_type = fx::core::detail::Matrix_Data;

  dimension_type specified_dims_;
  internal::Matrix_Size_Type allocated_dims_;
  int64_t total_elements_;
  //std::unique_ptr<buffer_type> data_;
//  buffer_type data_;
  friend struct fx::core::Matrix_Details;
  friend struct fx::core::Matrix::Details;

public:
  Details details;
  buffer_type data_;
  
  FX_SYMBOL_EXPORT T *data() noexcept;
  const T *data() const noexcept; 

  FX_SYMBOL_EXPORT explicit Matrix();
  FX_SYMBOL_EXPORT explicit Matrix(int64_t rows, int64_t cols);
  FX_SYMBOL_EXPORT explicit Matrix(dimension_type dims)
      : Matrix(dims.rows, dims.cols) {}
  //  template <typename Copy_Policy = shallow_copy_> Matrix(Matrix const &src,
  //  Copy_Policy) : Matrix(src, Copy_Policy()){}
  FX_SYMBOL_EXPORT Matrix(Matrix const &src); // : Matrix(src, shallow_copy) {}
  FX_SYMBOL_EXPORT Matrix(Matrix &&src);
  FX_SYMBOL_EXPORT Matrix(int64_t rows, int64_t cols,
                          std::vector<T> const &initial_data);
  FX_SYMBOL_EXPORT ~Matrix();
  // private:
  FX_SYMBOL_EXPORT Matrix(Matrix const &src, shallow_copy_);
  FX_SYMBOL_EXPORT Matrix(Matrix const &src, deep_copy_);

public:
  FX_SYMBOL_EXPORT friend void swap(Matrix &a, Matrix &b);

  FX_SYMBOL_EXPORT bool operator==(Matrix const &other);
  FX_SYMBOL_EXPORT bool operator==(Matrix const &other) const;
  FX_SYMBOL_EXPORT bool operator!=(Matrix const &other) {
    return !(operator==(other));
  }
  FX_SYMBOL_EXPORT bool operator!=(Matrix const &other) const {
    return !(operator==(other));
  }

  FX_SYMBOL_EXPORT Matrix &operator=(Matrix const &other);

  FX_SYMBOL_EXPORT T &operator()(int64_t row, int64_t col);
  FX_SYMBOL_EXPORT T operator()(int64_t row, int64_t col) const;

  FX_SYMBOL_EXPORT T &operator[](int64_t index);
  FX_SYMBOL_EXPORT T operator[](int64_t index) const;

  FX_SYMBOL_EXPORT matrix_iterator begin();
  FX_SYMBOL_EXPORT matrix_iterator begin() const;
  FX_SYMBOL_EXPORT matrix_iterator end();
  FX_SYMBOL_EXPORT matrix_iterator end() const;

private:
  // Serialization support: even if all of the code below runs on one
  // locality only, we need to provide an (empty) implementation for the
  // serialization as all arguments passed to actions have to support this.
  friend class hpx::serialization::access;
  //  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive &ar, const unsigned int version) {
    using std::get;
    int64_t s0, s1, a0, a1;
    s0 = specified_dims_.rows;
    s1 = specified_dims_.cols;
    a0 = allocated_dims_.major_dim;
    s1 = allocated_dims_.minor_dim;
    {
      ar &s0 &s1 &a0 &a1 &total_elements_ &data_;
      specified_dims_ = fx::core::Dimension_Type({s0, s1});
      allocated_dims_ = fx::core::internal::Matrix_Size_Type({a0, a1});
    }
  }

public:
  std::string Print() const;
  std::string Print() { return static_cast<Matrix const &>(*this).Print(); }


/*   class matrix_iterator { ... };   */
  #include "Matrix.Iterator.hpp"

};

void Zero_Padding_Elements(Matrix &m);

// template <typename OrderPolicy> int64_t Actual_Cols();
// template <typename OrderPolicy> int64_t Specified_Cols();

template <typename M>
int64_t Major_Dimension(M const &m_, Dimension_Type dim) {
  return fx::core::Matrix_Storage_Order_t<M>::Major_Dimension(dim);
}

template <typename M>
int64_t Minor_Dimension(M const &m_, Dimension_Type dim) {
  return fx::core::Matrix_Storage_Order_t<M>::Minor_Dimension(dim);
}

void swap(Matrix &a, Matrix &b);

    Matrix operator*(Matrix const&, Matrix const&);
    Matrix operator+(Matrix const&, Matrix const&);
  Matrix operator-(Matrix const&, Matrix const&);
  Matrix TimesPlus(Matrix const&, Matrix const&, Matrix const&);
  Matrix& Set_Row_Values(fx::core::Matrix &, int64_t, std::vector<float> const&);

  namespace vml_interface {
    fx::core::Matrix tanh(fx::core::Matrix const&);
  }

  namespace internal {
      template <typename M>
    struct Matrix_DataType{
      using type = typename M::data_type;
      };
  } // namespace internal
  
  //template <typename M>
  //  using Matrix_DataType = typename internal::Matrix_DataType::type;
  
  
} // namespace core
} // namespace fx

std::ostream &operator<<(std::ostream &, fx::core::Matrix const &);

#endif
