//#include "stdafx.h"
#include "Matrix.hpp"
#include "util/Exceptions.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>

bool fx::core::Matrix::matrix_iterator::
operator==(matrix_iterator const &other) const {
  // if both referenced matrices are set to nullptr, neither dereferences to a
  // valid matrix element (i.e. both point to data[size] -- 1 past the last
  // element)
  if ((mat_ == nullptr) && (other.mat_ == nullptr))
    return true;
  // check if only one iterator points to nullptr.  The case where both are
  // nullptr (and therefore equal) is handled before this line of code
  if ((mat_ == nullptr) || (other.mat_ == nullptr))
    return false;
  if (current_element == other.current_element)
    return true;
  return false;
}

bool fx::core::Matrix::matrix_iterator::
operator!=(matrix_iterator const &other) const {
  return !(this->operator==(other));
}

fx::core::Matrix::matrix_iterator::reference
    fx::core::Matrix::matrix_iterator::
    operator*() {
  if (mat_ != nullptr)
    return (*mat_)(current_element.first, current_element.second);
  else
    BOOST_THROW_EXCEPTION( std::runtime_error("Attempting to dereference a null pointer. This "
                             "iterator does not point at an element of the "
					      "matrix"));
}

fx::core::Matrix::matrix_iterator::reference
    fx::core::Matrix::matrix_iterator::
    operator*() const {
  if (mat_ != nullptr)
    return (*mat_)(current_element.first, current_element.second);
  else
    BOOST_THROW_EXCEPTION( std::runtime_error("Attempting to dereference a null pointer. This "
                             "iterator does not point at an element of the "
					      "matrix"));
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator++() {
  // if current row element is less than the total number of rows, then
  // increment the row
  if (current_element.first < matrix_dims_.first) {
    ++current_element.first;
  } else { // the current row is equal to the last row of the matrix.
    if (current_element.second < matrix_dims_.second) {
      current_element.first = 0, ++current_element.second;
    } else { // current row and current column are both at matrix_dims
      this->mat_ = nullptr;
    }
  }
  return *this;
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator--() {
  if (current_element.first > 0) {
    --current_element.first;
  } else { // current_element.first is at the top row of a given column
    if (current_element.first == 0) {
      if (current_element.second > 0) { // as long as we are not at (0,0), go to
                                        // the bottom of the previous column
        --current_element.second, current_element.first = matrix_dims_.first;
      } else // we are already at 0,0.  Decrementing this causes the iterator to
             // point to nullptr
      {
        this->mat_ = nullptr;
      }
    }
  }
  return *this;
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::matrix_iterator::
operator++(int) {
  matrix_iterator temp(*this);
  ++(*this);
  return temp;
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::matrix_iterator::
operator--(int) {
  matrix_iterator temp(*this);
  --(*this);
  return temp;
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator=(matrix_iterator other) {
  matrix_iterator temp_new(std::move(other));
  std::swap(*this, temp_new);
  return *this;
}

fx::core::Matrix::matrix_iterator::difference_type
    fx::core::Matrix::matrix_iterator::
    operator-(matrix_iterator const &other) {
  using std::get;

  // Note: current_element using 0-based indexing
  size_t other_element_idx =
      (other.current_element.first) +
      ((other.current_element.second)) * (other.matrix_dims_.first);
  int64_t this_element_idx =
      ((current_element.first)) +
      ((current_element.second)) * (matrix_dims_.first);

  return this_element_idx - other_element_idx;
}

#if 0
bool fx::core::Matrix::matrix_iterator::
operator==(matrix_iterator const &other) const {
  // if both referenced matrices are set to nullptr, neither dereferences to a
  // valid matrix element (i.e. both point to data[size] -- 1 past the last
  // element)
  if ((mat_ == nullptr) && (other.mat_ == nullptr))
    return true;
  // check if only one iterator points to nullptr.  The case where both are
  // nullptr (and therefore equal) is handled before this line of code
  if ((mat_ == nullptr) || (other.mat_ == nullptr))
    return false;
  if (current_element == other.current_element)
    return true;
  return false;
}

bool fx::core::Matrix::matrix_iterator::
operator!=(matrix_iterator const &other) const {
  return !(this->operator==(other));
}

fx::core::Matrix::matrix_iterator::reference
    fx::core::Matrix::matrix_iterator::
    operator*() {
  if (mat_ != nullptr)
    return (*mat_)(current_element.first, current_element.second);
  else
    BOOST_THROW_EXCEPTION(std::runtime_error("Attempting to dereference a null pointer. This "
                             "iterator does not point at an element of the "
					     "matrix"));
}

fx::core::Matrix::matrix_iterator::reference
    fx::core::Matrix::matrix_iterator::
    operator*() const {
  if (mat_ != nullptr)
    return (*mat_)(current_element.first, current_element.second);
  else
    BOOST_THROW_EXCEPTION( std::runtime_error("Attempting to dereference a null pointer. This "
                             "iterator does not point at an element of the "
					      "matrix"));
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator++() {
  // if current row element is less than the total number of rows, then
  // increment the row
  if (current_element.first < matrix_dims_.first) {
    ++current_element.first;
  } else { // the current row is equal to the last row of the matrix.
    if (current_element.second < matrix_dims_.second) {
      current_element.first = 0, ++current_element.second;
    } else { // current row and current column are both at matrix_dims
      this->mat_ = nullptr;
    }
  }
  return *this;
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator--() {
  if (current_element.first > 0) {
    --current_element.first;
  } else { // current_element.first is at the top row of a given column
    if (current_element.first == 0) {
      if (current_element.second > 0) { // as long as we are not at (0,0), go to
                                        // the bottom of the previous column
        --current_element.second, current_element.first = matrix_dims_.first;
      } else // we are already at 0,0.  Decrementing this causes the iterator to
             // point to nullptr
      {
        this->mat_ = nullptr;
      }
    }
  }
  return *this;
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::matrix_iterator::
operator++(int) {
  matrix_iterator temp(*this);
  ++(*this);
  return temp;
}

fx::core::Matrix::matrix_iterator fx::core::Matrix::matrix_iterator::
operator--(int) {
  matrix_iterator temp(*this);
  --(*this);
  return temp;
}

fx::core::Matrix::matrix_iterator &fx::core::Matrix::matrix_iterator::
operator=(matrix_iterator other) {
  matrix_iterator temp_new(std::move(other));
  std::swap(*this, temp_new);
  return *this;
}

fx::core::Matrix::matrix_iterator::difference_type
    fx::core::Matrix::matrix_iterator::
    operator-(matrix_iterator const &other) {
  using std::get;

  // Note: current_element using 0-based indexing
  int64_t other_element_idx =
      (other.current_element.first) +
      ((other.current_element.second)) * (other.matrix_dims_.first);
  int64_t this_element_idx =
      ((current_element.first)) +
      ((current_element.second)) * (matrix_dims_.first);

  return this_element_idx - other_element_idx;
}
#endif
