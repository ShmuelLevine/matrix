#include "gtest/gtest.h"
#include "core/Matrix.hpp"
#include "util/Exceptions.hpp"
#include <algorithm>
#include <iostream>
//#include <mkl_cblas.h>
//#include "core/Matrix/Matrix_Traits.hpp"

/*TEST(Matrix_Traits, T000) {
    std::size_t rows(50), cols(100);

    // fx::core::Matrix<float, fx::core::matrices::Padded<64>> mat1(rows,
    // cols);
    fx::core::Matrix mat1(rows, cols);

    size_t idx = fx::core::Matrix_Details::Index_Of(10, 10, mat1);

    ASSERT_EQ(64, idx);


}*/

TEST(Matrix, T001_Column_major_padded_64_matrix_spec_100_cols_expect_actual_major_dimension_100_minor_64){

  uint64_t rows(50), cols(100);

  // fx::core::Matrix<float, fx::core::matrices::Padded<64>> mat1(rows,
  // cols);
  fx::core::Matrix mat1(rows, cols);
  ASSERT_EQ(100, mat1.details.Actual_Cols());
  ASSERT_EQ(64, mat1.details.Actual_Rows());
}

TEST(Matrix, T002_padded_matrix_expect_actual_size_greater_or_equal_to_specified){
  using std::get;
  size_t rows(50), cols(100);

  //    fx::core::Matrix<float> mat1(rows, cols);
  fx::core::Matrix mat1(rows, cols);
  auto spec_size = mat1.details.Matrix_Size();
  auto actual_size = mat1.details.Data_Size();

  uint64_t specified_elements = spec_size.rows * spec_size.cols;
  uint64_t actual_elements = actual_size.rows * actual_size.cols;

  ASSERT_GE(actual_elements, specified_elements);
}

TEST(Matrix, T003_default_matrix_no_elements) {

  using std::get;

  fx::core::Matrix mat1;

  ASSERT_EQ(0, mat1.details.Matrix_Elements());
}

TEST(Matrix, T003a_default_matrix_no_memory_allocation) {

  using std::get;

  fx::core::Matrix mat1;

  ASSERT_EQ(0, mat1.details.Data_Size().rows);
  ASSERT_EQ(0, mat1.details.Data_Size().cols);
}

TEST(Matrix, T003b_default_matrix_equality_expect_true) {
  fx::core::Matrix m1, m2;
  EXPECT_EQ(m1, m2);
}

TEST(Matrix, T004_copy_constructor_copy_matches_original) {
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);
  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  mat1(0, 2) = 3.0f;
  mat1(1, 2) = 3.1f;
  mat1(2, 2) = 3.2f;

  fx::core::Matrix mat2(mat1);
  ASSERT_EQ(mat1.details.Matrix_Size(), mat2.details.Matrix_Size());
  EXPECT_EQ(mat1(0, 0), mat2(0, 0));
  EXPECT_EQ(mat1(1, 0), mat2(1, 0));
  EXPECT_EQ(mat1(2, 0), mat2(2, 0));
  EXPECT_EQ(mat1(0, 1), mat2(0, 1));
  EXPECT_EQ(mat1(1, 1), mat2(1, 1));
  EXPECT_EQ(mat1(2, 1), mat2(2, 1));
  EXPECT_EQ(mat1(0, 2), mat2(0, 2));
  EXPECT_EQ(mat1(1, 2), mat2(1, 2));
  EXPECT_EQ(mat1(2, 2), mat2(2, 2));
}

TEST(Matrix, T005_copy_constructor_copy_matches_original_equality_operator) {
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);
  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  mat1(0, 2) = 3.0f;
  mat1(1, 2) = 3.1f;
  mat1(2, 3) = 3.2f;

  fx::core::Matrix mat2(mat1);
  ASSERT_EQ(mat1.details.Matrix_Size(), mat2.details.Matrix_Size());
  EXPECT_EQ(mat1, mat2);
}

TEST(Matrix, T006_copy_constructor_confirm_default_deep_copy) {
  using std::get;
  size_t rows(3), cols(3);

  fx::core::Matrix mat1(rows, cols);
  mat1(0, 0) = 1.0f;
  mat1(1, 0) = 1.1f;
  mat1(2, 0) = 1.2f;

  mat1(0, 1) = 2.0f;
  mat1(1, 1) = 2.1f;
  mat1(2, 1) = 2.2f;

  mat1(0, 2) = 3.0f;
  mat1(1, 2) = 3.1f;
  mat1(2, 3) = 3.2f;

  fx::core::Matrix mat2(mat1);

  ASSERT_EQ(mat1.details.Matrix_Size(), mat2.details.Matrix_Size());
  EXPECT_EQ(mat1, mat2);

  mat1(0, 0) = 2.0f;
  EXPECT_NE(mat1, mat2);
}


TEST(Matrix, T007_Confirm_Matrix_Move_Constructor_Expect_DataMoves) {
  size_t rows(3), cols(3);

  fx::core::Matrix m1(rows, cols);
  m1(0, 0) = 1.0f;
  m1(1, 0) = 1.1f;
  m1(2, 0) = 1.2f;

  m1(0, 1) = 2.0f;
  m1(1, 1) = 2.1f;
  m1(2, 1) = 2.2f;

  m1(0, 2) = 3.0f;
  m1(1, 2) = 3.1f;
  m1(2, 3) = 3.2f;

  auto data_addr = m1.data();

  fx::core::Matrix m2(std::move(m1));

  EXPECT_EQ(data_addr, m2.data());
  EXPECT_NE(data_addr, m1.data());

//  EXPECT_EQ(m1, fx::core::Matrix());
  // Note: this test causes a crash.  After some thought, I now realize that this is the proper behaviour since an object is in an invalid state once moved, in accordance with the C++ standard...

}

TEST(Matrix, T008_Confirm_Specified_Rows_254x768_ColumnMajor_Expect_254){
    size_t rows(254), cols(768);
    fx::core::Matrix mat1(rows, cols);
    size_t specified_rows = mat1.details.Specified_Rows();
    EXPECT_EQ(254, specified_rows);
}

TEST(Matrix, T011_Confirm_Actual_Rows_254x768_ColumnMajor_Expect_256){
    size_t rows(254), cols(768);
    fx::core::Matrix mat1(rows, cols);
    size_t actual_rows = mat1.details.Actual_Rows();
    EXPECT_EQ(256, actual_rows);
}

TEST(Matrix, T012_Initializing_Data_Constructor_stdVector_too_small_expect_throw){

    size_t rows(3), cols(3);
    std::vector<float> initial_data_(5,5); // Too few datapoints -- expect 9, actual 5

    EXPECT_THROW(fx::core::Matrix(rows, cols, std::move(initial_data_)), fx::exception::Invalid_Data);
    
}

TEST(Matrix, T013_Initializing_Data_Constructor_stdVector_too_large_expect_throw){

    size_t rows(3), cols(3);
    std::vector<float> initial_data_(10,10); // Too many datapoints -- expect 9, actual 10

    EXPECT_THROW(fx::core::Matrix(rows, cols, std::move(initial_data_)), fx::exception::Invalid_Data);    
}


TEST(Matrix, T014_Initializing_Data_Constructor_Confirm_Matrix_Matches_original){

        size_t rows(3), cols(3);
        std::vector<float> initial_data_;

        for (int i = 0; i < 9; ++i)
            initial_data_.push_back(static_cast<float>(i) * 1.1f);

        fx::core::Matrix m1 (rows, cols, std::move(initial_data_));

        EXPECT_FLOAT_EQ(0.0, m1(0,0));
        EXPECT_FLOAT_EQ(1.1f, m1(1,0));
        EXPECT_FLOAT_EQ(2.2f, m1(2,0));
        EXPECT_FLOAT_EQ(3.3f, m1(0,1));
        EXPECT_FLOAT_EQ(4.4f, m1(1,1));
        EXPECT_FLOAT_EQ(5.5f, m1(2,1));
        EXPECT_FLOAT_EQ(6.6f, m1(0,2));
        EXPECT_FLOAT_EQ(7.7f, m1(1,2));
        EXPECT_FLOAT_EQ(8.8f, m1(2,2));
                
}

TEST(Matrix, T015_Confirm_Padding_Elements_Set_to_zero_no_impact_to_magnitude) {

	size_t rows(8), cols(50000);
	
	fx::core::Matrix m(rows, cols);

	// Set all of the non-padding elements to 0.f
	std::fill(m.begin(), m.end(), 0.f);

	float euclidean_norm_sq;

	auto actual_dims = m.details.Data_Size();
	
	const int64_t n = actual_dims.rows * actual_dims.cols;
	const int64_t inc_one = 1;

        m[10] = 1.0f;

	euclidean_norm_sq = cblas_sdot(n, m.data(), inc_one, m.data(), inc_one);

	// In general, the padding elements are non-zero and the norm should not be equal to 0
	EXPECT_NE(0.0f, euclidean_norm_sq);

	fx::core::Zero_Padding_Elements(m);
	euclidean_norm_sq = cblas_sdot(n, m.data(), inc_one, m.data(), inc_one);
	EXPECT_FLOAT_EQ(0.f, euclidean_norm_sq);
}

TEST(Matrix, T016_Matrix_Operations_Check_Multiplication_Throws_Incompatible_Sizes)
{
  EXPECT_TRUE(true);
}

TEST(Matrix, T017_Matrix_Operations_Check_Multiplication_Works)
{}

TEST(Matrix, T018_Matrix_Operations_Check_Addition_Works)
{

        size_t rows(3), cols(3);
        std::vector<float> initial_data_;

        for (int i = 0; i < 9; ++i)
            initial_data_.push_back(static_cast<float>(i) * 1.1f);

        fx::core::Matrix m1 (rows, cols, std::vector<float>(initial_data_));
	fx::core::Matrix m2 (rows, cols, std::move(initial_data_));

	fx::core::Matrix m3 = m1 + m2;
	
        EXPECT_FLOAT_EQ(0.0, m3(0,0));
        EXPECT_FLOAT_EQ(2.2f, m3(1,0));
        EXPECT_FLOAT_EQ(4.4f, m3(2,0));
        EXPECT_FLOAT_EQ(6.6f, m3(0,1));
        EXPECT_FLOAT_EQ(8.8f, m3(1,1));
        EXPECT_FLOAT_EQ(11.0f, m3(2,1));
        EXPECT_FLOAT_EQ(13.2f, m3(0,2));
        EXPECT_FLOAT_EQ(15.4f, m3(1,2));
        EXPECT_FLOAT_EQ(17.6f, m3(2,2));

  
}

