//#include "stdafx.h"
#include "Matrix.hpp"
#include "Matrix_Data.hpp"
#include "util/Exceptions.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <memory>
#include <mkl_blas.h>
#include <mkl_vml.h>
//#include <hpx/parallel/algorithms/transform.hpp>
#include <hpx/include/parallel_transform.hpp>
#include <hpx/include/parallel_execution_policy.hpp>
#include <functional>

#ifdef _MSC_VER
#define VML_VSADD vsadd
#define VML_VSSUB vssub
#define VML_TANH vstanh
#else
#define VML_VSADD vsadd_
#define VML_VSSUB vssub_
#define VML_TANH vstanh_
#endif

constexpr int64_t Index_Of(int64_t rows, int64_t cols) {
  return FX_SYMBOL_EXPORT int64_t();
}

fx::core::Matrix::Matrix()
    : specified_dims_({0, 0}), allocated_dims_({0, 0}), total_elements_(0),
      data_(), details(*this) {}

fx::core::Matrix::Matrix(int64_t rows, int64_t cols)
    : specified_dims_({rows, cols}),
      allocated_dims_({Order_Policy::Major_Dimension(specified_dims_),
                       Padding_Policy::Padded_Length(
                           Order_Policy::Minor_Dimension(specified_dims_))}),
      total_elements_(allocated_dims_.major_dim * allocated_dims_.minor_dim),
      data_{detail::Matrix_Data(total_elements_)}, details(*this) {}

fx::core::Matrix::Matrix(Matrix const &src)
    : fx::core::Matrix(src, fx::core::deep_copy) {}

fx::core::Matrix::Matrix(Matrix const &src, fx::core::shallow_copy_)
    : specified_dims_(src.specified_dims_),
      allocated_dims_({Order_Policy::Major_Dimension(specified_dims_),
                       Padding_Policy::Padded_Length(
                           Order_Policy::Minor_Dimension(specified_dims_))}),
      total_elements_(allocated_dims_.major_dim * allocated_dims_.minor_dim),
      data_{detail::Matrix_Data((src.data_), fx::core::shallow_copy)},
      details(*this) {}

fx::core::Matrix::Matrix(Matrix const &src, fx::core::deep_copy_)
    : specified_dims_(src.specified_dims_),
      allocated_dims_({Order_Policy::Major_Dimension(specified_dims_),
                       Padding_Policy::Padded_Length(
                           Order_Policy::Minor_Dimension(specified_dims_))}),
      total_elements_(allocated_dims_.major_dim * allocated_dims_.minor_dim),
      data_{detail::Matrix_Data((src.data_), fx::core::deep_copy)},
      details(*this) {}

fx::core::Matrix::Matrix(Matrix &&src)
    : specified_dims_(std::move(src.specified_dims_)),
      allocated_dims_(std::move(src.allocated_dims_)),
      total_elements_(std::move(src.total_elements_)),
      data_(std::move(src.data_)), details(*this) {
  //  fx::core::Matrix tmp;
  //  swap(tmp, src);
  src.data_ = detail::Matrix_Data();
}

fx::core::Matrix::~Matrix() {
  // if (data_ != nullptr)
  //   delete(data_);
}

fx::core::Matrix::T &fx::core::Matrix::operator[](int64_t index) {
  return data_.operator[](index);
}
fx::core::Matrix::T fx::core::Matrix::operator[](int64_t index) const {
  return T{data_.operator[](index)};
}

// template <typename CP> fx::core::Matrix::Matrix<CP>(fx::core::Matrix const&
// src, CP) : Matrix(src, CP()) {}

fx::core::Matrix::Matrix(int64_t rows, int64_t cols,
                         std::vector<T> const &initial_data)
    : Matrix(rows, cols) {

  namespace fe = fx::exception;
  int64_t data_elements = specified_dims_.rows * specified_dims_.cols;
  if (initial_data.size() != data_elements)
    throw(fe::Invalid_Data() << fe::datapoints_min(data_elements)
                             << fe::datapoints_available(initial_data.size()));

  // Note: cannot use std::copy, since it will use memcpy and not the
  // Matrix_Iterator
  // std::copy(initial_data.begin(), initial_data.end(), &data_[0]);
  auto src_iter = initial_data.begin();
  auto src_end = initial_data.end();
  auto tgt_iter = begin();
  auto tgt_end = end();
  while ((src_iter != src_end) && (tgt_iter != tgt_end)) {
    // post-increment both iterators -- i.e. first copy the data and then
    // increment
    *tgt_iter++ = *src_iter++;
  }
}

bool fx::core::Matrix::operator==(Matrix const &other) const {
  // Note: using std::equal will provide erroneous results since (1) different
  // matrices might have different padding lengths but the matrix size and
  // contents are the same;
  //       and (2) memory elements [R,*] where specified_dims_<0> < R <=
  //       actual_dims_<0>  (i.e. in the memory regions containing the padding
  //       elements.  In general,
  //       these elements are uninitialized data and may contain anything.
  // return std::equal(&data_[0], &data_[total_elements_], other.data_[0]) ];

  // The region of equality are all points within the matrix data partition of
  // the allocated memory
  // (1) the specified_dims are not the

  using std::get;
  if (specified_dims_ != other.specified_dims_)
    return false;
  if ((this->total_elements_ == 0) && (other.total_elements_ == 0))
    return true;
  auto this_iter = begin();

  //   total_elements_ = get<0>(specified_dims_) * get<1>(specified_dims_);
  return std::equal(this_iter, end(), other.begin());
}

bool fx::core::Matrix::operator==(Matrix const &other) {
  return static_cast<const Matrix &>(*this).operator==(other);
}

//    template <typename T> int64_t  Minor_Dimension(T const&);

void fx::core::Zero_Padding_Elements(Matrix &m) {

  int64_t actual_minor = fx::core::Minor_Dimension(m, m.details.Data_Size());
  int64_t spec_minor{fx::core::Minor_Dimension(m, m.details.Matrix_Size())};
  int64_t actual_major{fx::core::Major_Dimension(m, m.details.Data_Size())};
  ; // only rows are needed for column-major

  int64_t first_padding_element = spec_minor;

  int64_t element_idx;
  auto matrix = m.data();

  for (auto col_idx = 0; col_idx < actual_major; ++col_idx) {
    element_idx = first_padding_element + col_idx * actual_minor;
    for (auto row_idx = first_padding_element; row_idx < actual_minor;
         ++row_idx, ++element_idx)
      matrix[element_idx] = 0.0f;
  }
}

void fx::core::swap(Matrix &a, Matrix &b) {
  // Use std::swap for built-in types
  using std::swap;
  swap(a.specified_dims_, b.specified_dims_);
  swap(a.allocated_dims_, b.allocated_dims_);
  swap(a.total_elements_, b.total_elements_);
  swap(a.data_, b.data_);
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::begin() {
  return fx::core::Matrix::matrix_iterator(this);
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::end() {
  return fx::core::Matrix::matrix_iterator();
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::begin() const {
  const auto c_iter = fx::core::Matrix::matrix_iterator(this);
  return c_iter;
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::end() const {
  return fx::core::Matrix::matrix_iterator();
}

fx::core::Matrix::T *fx::core::Matrix::data() noexcept { return data_.Data(); }
const fx::core::Matrix::T *fx::core::Matrix::data() const noexcept {
  return data_.Data();
}

namespace fx {
namespace core {

// template <>
// FX_SYMBOL_EXPORT void fx::core::Matrix<matrices::Row_Major>::Print()
/* Not templated now -- all matrices are column-major */
std::string fx::core::Matrix::Print() const {
  std::stringstream ss;
  const int field_width = 8;

  auto specified_rows = details.Specified_Rows();
  int64_t specified_cols = details.Specified_Cols();
  int64_t actual_rows = details.Actual_Rows();

  for (int64_t r = 0; r < specified_rows; ++r) {
    for (int64_t c = 0; c < specified_cols; ++c) {
      ss << std::setprecision(5) << std::setw(field_width)
         << data_.operator[](c * actual_rows + r);
    }
    ss << std::endl;
  }
  ss << std::endl;
  return ss.str();
}

} // namespace core
} // namespace fx

std::ostream &operator<<(std::ostream &os, fx::core::Matrix const &m) {
  os << m.Print();
  return os;
}

FX_SYMBOL_EXPORT fx::core::Matrix &fx::core::Matrix::
operator=(Matrix const &other) {
  Matrix t{other};
  swap(t, *this);
  return *this;
}

FX_SYMBOL_EXPORT fx::core::Matrix::T &fx::core::Matrix::operator()(int64_t row,
                                                                   int64_t col) {
  return data_.operator[](details.Index_Of(row, col));
}
FX_SYMBOL_EXPORT fx::core::Matrix::T fx::core::Matrix::
operator()(int64_t row, int64_t col) const {
  return T{data_.operator[](details.Index_Of(row, col))};
}

fx::core::Matrix fx::core::operator*(fx::core::Matrix const &m1,
                                     fx::core::Matrix const &m2) {
  const MKL_INT m1_rows = m1.details.Specified_Rows();
  const MKL_INT m1_cols = m1.details.Specified_Cols();
  const MKL_INT m2_rows = m2.details.Specified_Rows();
  const MKL_INT m2_cols = m2.details.Specified_Cols();
  const MKL_INT m1_leading_dim = m1.details.Leading_Dim();
  const MKL_INT m2_leading_dim = m2.details.Leading_Dim();

  if (m1_cols != m2_rows)
    BOOST_THROW_EXCEPTION(fx::exception::Runtime_Error() << fx::exception::exception_details(
        "The parameters are invalid: matrix-matrix multiplication is undefined "
        "where the number of rows in the 2nd matrix is not equal to the number "
        "of columns in the first matrix")) ;

  fx::core::Matrix m3(m1_rows, m2_cols);
  const MKL_INT m3_leading_dim = m3.details.Leading_Dim();

  const float alpha = 1.0f, beta = 0.0f;

  
  SGEMM("N", "N", &m1_rows, &m2_cols, &m1_cols, &alpha, m1.data(), &m1_leading_dim, m2.data(), &m2_leading_dim, &beta, m3.data(), &m3_leading_dim);

  return m3;
}

fx::core::Matrix fx::core::operator+(fx::core::Matrix const &m1,
                                     fx::core::Matrix const &m2) {
  auto m1_spec_size = m1.details.Matrix_Size();
  auto m2_spec_size = m2.details.Matrix_Size();

  if (m1_spec_size != m2_spec_size){
    BOOST_THROW_EXCEPTION( fx::exception::Runtime_Error() << fx::exception::exception_details(
			    "The parameters are invalid: both matrices must be the same size"));
  }

  auto m1_data_size = m1.details.Data_Size();
  auto m2_data_size = m2.details.Data_Size();


  
  fx::core::Matrix m3(m1_spec_size);
  
  if ( (m1_data_size == m2_data_size) && (m1_data_size == m3.details.Data_Size())){
    const MKL_INT n_elements = m1.details.Data_Elements();
    VML_VSADD(&n_elements, m1.data(), m2.data(), m3.data());
    return m3;
  }

  //  hpx::parallel::transform(hpx::parallel::par_vec, m1.begin(), m1.end(), m2.begin(), m3.begin(), [&](auto f1, auto f2){ return f1 + f2;});
    std::transform(m1.begin(), m1.end(), m2.begin(), m3.begin(), [&](auto f1, auto f2){ return f1 + f2;});
  return m3;

}

fx::core::Matrix fx::core::operator-(fx::core::Matrix const &m1,
                                     fx::core::Matrix const &m2) {
  auto m1_spec_size = m1.details.Matrix_Size();
  auto m2_spec_size = m2.details.Matrix_Size();

  if (m1_spec_size != m2_spec_size){
    BOOST_THROW_EXCEPTION( fx::exception::Runtime_Error() << fx::exception::exception_details(
			    "The parameters are invalid: both matrices must be the same size"));
  }

  auto m1_data_size = m1.details.Data_Size();
  auto m2_data_size = m2.details.Data_Size();


  
  fx::core::Matrix m3(m1_spec_size);
  
  if ( (m1_data_size == m2_data_size) && (m1_data_size == m3.details.Data_Size())){
    const MKL_INT n_elements = m1.details.Data_Elements();
    VML_VSSUB(&n_elements, m1.data(), m2.data(), m3.data());
    return m3;
  }

  //  hpx::parallel::transform(hpx::parallel::par_vec, m1.begin(), m1.end(), m2.begin(), m3.begin(), [&](auto f1, auto f2){ return f1 - f2;});
    std::transform( m1.begin(), m1.end(), m2.begin(), m3.begin(), [&](auto f1, auto f2){ return f1 - f2;});
  return m3;

}

fx::core::Matrix fx::core::TimesPlus(Matrix const& m1, Matrix const& m2, Matrix const& m3)
{
    const MKL_INT m1_rows = m1.details.Specified_Rows();
    const MKL_INT m1_cols = m1.details.Specified_Cols();
    const MKL_INT m2_rows = m2.details.Specified_Rows();
    const MKL_INT m2_cols = m2.details.Specified_Cols();
    const MKL_INT m1_leading_dim = m1.details.Leading_Dim();
    const MKL_INT m2_leading_dim = m2.details.Leading_Dim();

    if (m1_cols != m2_rows)
        BOOST_THROW_EXCEPTION(fx::exception::Runtime_Error() << fx::exception::exception_details(
            "The parameters are invalid: matrix-matrix multiplication is undefined "
            "where the number of rows in the 2nd matrix is not equal to the number "
            "of columns in the first matrix"));

    fx::core::Matrix m4(m1_rows, m2_cols);
    const MKL_INT m4_leading_dim = m4.details.Leading_Dim();
    const MKL_INT m4_total_elements = m4.details.Data_Elements();
    const float alpha = 1.0f, beta = 1.0f;

    cblas_scopy(m4_total_elements, m3.data(), 1, m4.data(), 1);

    SGEMM("N", "N", &m1_rows, &m2_cols, &m1_cols, &alpha, m1.data(), &m1_leading_dim, m2.data(), &m2_leading_dim, &beta, m4.data(), &m4_leading_dim);

    return m4;

}

// Row index uses 0-based indexing
fx::core::Matrix& fx::core::Set_Row_Values(fx::core::Matrix& m, int64_t row, std::vector<float> const& values)
{
#ifdef DEBUG
    if (m.details.Actual_Cols() != values.size())
        BOOST_THROW_EXCEPTION(fx::exception::Runtime_Error() << fx::exception::exception_details("Cannot set the row values: the number of elements in the row differs from the number of elements in the vector."));
    if (m.details.Specified_Rows() > row)
        BOOST_THROW_EXCEPTION(fx::exception::Runtime_Error() << fx::exception::exception_details("Invalid row index") << fx::exception::specified_index(row) << fx::exception::matrix_total_rows(m.details.Specified_Rows()));
#endif

    const MKL_INT n_elements = m.details.Actual_Cols();
    float* starting_datum = m.data() + row;

    // XXX: hard-coded for column-major right now.  The stride for elements in a single row
    // is equal to the total number of elements.  Starting point is the matrix data point plus the offset for the row
    cblas_scopy(n_elements, values.data(), 1, starting_datum, m.details.Actual_Rows());
    
}

fx::core::Matrix fx::core::vml_interface::tanh(fx::core::Matrix const& m) {

  fx::core::Matrix outputs {m.details.Matrix_Size() };
  HPX_ASSERT (m.details.Data_Size() == outputs.details.Data_Size());
  const MKL_INT n_elements = m.details.Data_Elements();
  VML_TANH( &n_elements, m.data(), outputs.data());
  return outputs;
  
  
}
