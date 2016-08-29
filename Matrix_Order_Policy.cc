#include "Matrix_Order_Policy.hpp"


 int64_t fx::core::matrices::Column_Major::Major_Dimension(fx::core::Dimension_Type const& dims) noexcept {
    return dims.cols;
  }
 int64_t fx::core::matrices::Column_Major::Minor_Dimension(fx::core::Dimension_Type const& dims) noexcept {
    return dims.rows;
  }

int64_t fx::core::matrices::Column_Major::Rows(fx::core::internal::Matrix_Size_Type const& dim_maj_min) noexcept{
  return dim_maj_min.minor_dim;
}
int64_t fx::core::matrices::Column_Major::Cols(fx::core::internal::Matrix_Size_Type const& dim_maj_min) noexcept{
  return dim_maj_min.major_dim;
}


 int64_t fx::core::matrices::Row_Major::Major_Dimension(fx::core::Dimension_Type const& dims) noexcept {
    return dims.rows;
  }
 int64_t fx::core::matrices::Row_Major::Minor_Dimension(fx::core::Dimension_Type const& dims) noexcept {
    return dims.cols;
  }
int64_t fx::core::matrices::Row_Major::Rows(fx::core::internal::Matrix_Size_Type const& dim_maj_min) noexcept{
  return dim_maj_min.major_dim;
}
int64_t fx::core::matrices::Row_Major::Cols(fx::core::internal::Matrix_Size_Type const& dim_maj_min) noexcept{
  return dim_maj_min.minor_dim;
}
