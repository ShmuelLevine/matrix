//#include "stdafx.h"
#include "Matrix.hpp"
#include "util/Exceptions.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>


FX_EXPORT int64_t fx::core::Matrix::Details::Specified_Rows() const noexcept
{
    return m_.specified_dims_.rows;
}


int64_t fx::core::Matrix::Details::Actual_Rows() const noexcept
{
//  return this->Actual_Rows_(m_);
  return fx::core::internal::Utility_Functions::MajorMinor_To_RowsCols(m_, m_.allocated_dims_).rows;
}
int64_t fx::core::Matrix::Details::Actual_Cols() const noexcept 
{
   return fx::core::internal::Utility_Functions::MajorMinor_To_RowsCols(m_, m_.allocated_dims_).cols;
}

FX_EXPORT int64_t fx::core::Matrix::Details::Specified_Cols() const noexcept
{
    return m_.specified_dims_.cols;
}


        FX_SYMBOL_EXPORT int64_t fx::core::Matrix::Details::Actual_Rows() {
            return static_cast<fx::core::Matrix::Details const&>(*this).Actual_Rows();
        }

        FX_SYMBOL_EXPORT int64_t fx::core::Matrix::Details::Specified_Rows() {
            return static_cast<fx::core::Matrix::Details const&>(*this).Specified_Rows();
        }

        FX_SYMBOL_EXPORT int64_t fx::core::Matrix::Details::Actual_Cols() {
            return static_cast<fx::core::Matrix::Details const&>(*this).Actual_Cols();
        }

        FX_SYMBOL_EXPORT int64_t fx::core::Matrix::Details::Specified_Cols() {
            return static_cast<fx::core::Matrix::Details const&>(*this).Specified_Cols();
        }
         int64_t fx::core::Matrix::Details::Index_Of(int64_t rows, int64_t cols) const {
/*	   return fx::core::Major_Dimension(m_, fx::core::Dimension_Type{rows, cols}) *
	     fx::core::Major_Dimension(m_, m_.allocated_dims_) +
	     fx::core::Minor_Dimension(m_, fx::core::Dimension_Type{rows, cols});
*/

	   auto stride = m_.allocated_dims_.minor_dim;
	   auto requested_index = fx::core::Dimension_Type{rows, cols};
	   
	   return fx::core::Major_Dimension(m_, requested_index ) *
	     stride + fx::core::Minor_Dimension(m_, requested_index );
        }

         int64_t fx::core::Matrix::Details::Index_Of(int64_t rows, int64_t cols)  {
	  return static_cast<fx::core::Matrix::Details const&>(*this).Index_Of(rows, cols);
	}


        fx::core::Matrix::dimension_type fx::core::Matrix::Details::Matrix_Size() const noexcept {
            return m_.specified_dims_;
        }
fx::core::Matrix::dimension_type fx::core::Matrix::Details::Data_Size() const noexcept { return fx::core::internal::Utility_Functions::MajorMinor_To_RowsCols(m_, m_.allocated_dims_); }

        int64_t fx::core::Matrix::Details::Leading_Dim() const noexcept {
//	  return fx::core::Minor_Dimension(m_, m_.allocated_dims_);
    return m_.allocated_dims_.minor_dim;
        }

         int64_t fx::core::Matrix::Details::Matrix_Elements()  const noexcept {
            return m_.total_elements_;
        }

int64_t fx::core::Matrix::Details::Data_Elements() const noexcept {
  return m_.allocated_dims_.major_dim *  m_.allocated_dims_.minor_dim;
}
