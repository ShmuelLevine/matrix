#include "gtest/gtest.h"
#include "core/Matrix.hpp"
#include "util/Exceptions.hpp"
#include <algorithm>
#include <iostream>


TEST(Matrix_iterator, T001_iterator_point_first_element) {
  using std::get;
  size_t rows(3), cols(3);

  //    fx::core::Matrix<float> mat1(rows, cols);
  fx::core::Matrix mat1(rows, cols);
  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  EXPECT_EQ(1.0f, *iter);
}

TEST(Matrix_iterator, T002_iterator_increment_point_next_element) {
  using std::get;
  size_t rows(3), cols(3);

  //    fx::core::Matrix<float> mat1(rows, cols);
  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  ++iter;
  EXPECT_EQ(1.1f, *iter);
}

TEST(Matrix_iterator, T003_iterator_increment_decrement_point_same_element) {
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  ++iter;
  --iter;
  EXPECT_EQ(1.0f, *iter);
}

TEST(Matrix_iterator, T004_iterator_increment_past_end_equals_default_iterator){
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  size_t n_elements = rows * cols;
  for (int i = 0; i < n_elements; ++i)
    ++iter;

  // increment 1 more time to past end of the matrix
  ++iter;
  fx::core::Matrix::matrix_iterator iter2;
  EXPECT_EQ(iter2, iter);
}

TEST(Matrix_iterator, T005_iterator_decrement_before_begin_equals_default_iterator){
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  --iter;

  fx::core::Matrix::matrix_iterator iter2;
  EXPECT_EQ(iter2, iter);
}

TEST(Matrix_iterator, T006_iterator_increment_decrement_0_0_expect_nullptr) {
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  size_t n_elements = rows * cols;
  for (int i = 0; i < n_elements; ++i)
    ++iter;

  // increment 1 more time to past end of the matrix
  ++iter;
  --iter; // decrement the null iterator
  fx::core::Matrix::matrix_iterator iter2;
  EXPECT_EQ(iter2, iter); // should stay null.
}

TEST(Matrix_iterator, T007_iterator_increment_copy_constructor_newcopy_matches_original){
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
//  size_t n_elements = rows * cols;
  ++iter;

  decltype(iter) iter2(iter);

  EXPECT_EQ(iter2, iter);
  EXPECT_EQ(*iter, *iter2);
}

TEST(Matrix_iterator, T008_iterator_increment_copy_constructor_different_pointers){
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
//  size_t n_elements = rows * cols;
  ++iter;

  decltype(iter) iter2(iter);
  ++iter;
  EXPECT_NE(iter2, iter);
  EXPECT_NE(*iter, *iter2);
}

TEST(Matrix_iterator, T009_iterator_dereference_nullptr_expect_throw) {
  fx::core::Matrix::matrix_iterator iter;
  EXPECT_THROW(*iter, std::exception);
}

TEST(Matrix_iterator, T010_prefix_and_postfix_increment_behaves_as_expected) {
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;
  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();

  // iter starts at 1.0f
  EXPECT_EQ(1.1f, *(++iter));
  EXPECT_EQ(1.1f, *(iter++));
  EXPECT_EQ(1.2f, *iter);
}

TEST(Matrix_iterator, T011_prefix_and_postfix_increment_behaves_as_expected) {
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;
  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();

  // iter starts at 1.0f
  EXPECT_EQ(1.1f, *(++iter));
  EXPECT_EQ(1.1f, *(iter++));
  EXPECT_EQ(1.2f, *iter);
}

TEST(Matrix_iterator, T012_prefix_and_postfix_decrement_behaves_as_expected) {
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;
  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  fx::core::Matrix::matrix_iterator iter = mat1.begin();
  for (int i = 0; i < 4; ++i)
    ++iter; // increment the iterator to element (1,1)

  EXPECT_EQ(2.1f, *iter);
  EXPECT_EQ(2.0f, *(--iter));
  EXPECT_EQ(2.0f, *(iter--));
  EXPECT_EQ(1.2f, *iter);

  // iter starts at 1.0f
}

TEST(Matrix_iterator, T013_iterator_difference_less_or_equal_to_pointer_difference){
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);

  /*
      mat1(0, 0) = 1.0f;
      mat1(1, 0) = 1.1f;
      mat1(2, 0) = 1.2f;
      mat1(0, 1) = 2.0f;
      mat1(1, 1) = 2.1f;
      mat1(2, 1) = 2.2f;
  */

  fx::core::Matrix::matrix_iterator iter1 = mat1.begin();
  fx::core::Matrix::matrix_iterator iter2 = mat1.begin();
  for (int i = 0; i < 6; ++i)
    ++iter1; // increment the iterator to element (1,1)

  auto iter_difference = iter1 - iter2;
  auto ptr_difference = &*iter1 - &*iter2;

  EXPECT_LE(iter_difference, ptr_difference);

  // iter starts at 1.0f
}
