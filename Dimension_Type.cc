#include "Dimension_Type.hpp"
#include "Matrix_Order_Policy.hpp"
#include "Matrix.hpp"

  bool fx::core::operator== (fx::core::Dimension_Type const& a, fx::core::Dimension_Type const& b)
{
    return (a.cols == b.cols) && (a.rows == b.rows);
}
bool  fx::core::operator!= (fx::core::Dimension_Type const& a, fx::core::Dimension_Type const& b)
{
    return !(a == b);
}

bool fx::core::internal::operator==(fx::core::internal::Matrix_Size_Type const& a, fx::core::internal::Matrix_Size_Type const& b) {
  return (a.major_dim == b.major_dim) && (a.minor_dim == b.minor_dim);
}

bool fx::core::internal::operator!= (fx::core::internal::Matrix_Size_Type const& a, fx::core::internal::Matrix_Size_Type const& b) {
  return !(a==b);
}

      template <typename M>
      fx::core::Dimension_Type fx::core::internal::Utility_Functions::MajorMinor_To_RowsCols(M, fx::core::internal::Matrix_Size_Type dim){
	return fx::core::Dimension_Type({	fx::core::Matrix_Storage_Order_t<M>::Rows(dim), fx::core::Matrix_Storage_Order_t<M>::Cols(dim)});
      }

template
fx::core::Dimension_Type fx::core::internal::Utility_Functions::MajorMinor_To_RowsCols<fx::core::Matrix>(fx::core::Matrix, fx::core::internal::Matrix_Size_Type dim);
