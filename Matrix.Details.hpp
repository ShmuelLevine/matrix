/*

This file is intended to be included by Matrix.hpp; this struct is a member of fx::core::Matrix.

This declaration is included in this file for the purpose of keeping the Matrix class more clear, and for facilitating the identification of the members of this struct, since it can be difficult to locate it within the declaration of Matrix.

 */

struct Details {

    Details(fx::core::Matrix const &m) : m_{m} {}

    // template <typename Order_Policy>
    FX_SYMBOL_EXPORT int64_t Index_Of(int64_t rows, int64_t cols);
    FX_SYMBOL_EXPORT int64_t Index_Of(int64_t rows, int64_t cols) const;

    FX_SYMBOL_EXPORT dimension_type Matrix_Size() const noexcept;
    FX_SYMBOL_EXPORT dimension_type Data_Size() const noexcept;
    FX_SYMBOL_EXPORT int64_t Leading_Dim() const noexcept;
    FX_SYMBOL_EXPORT int64_t Matrix_Elements() const noexcept;

    FX_SYMBOL_EXPORT int64_t Actual_Rows() const noexcept;
    FX_SYMBOL_EXPORT int64_t Actual_Cols() const noexcept;
    FX_SYMBOL_EXPORT int64_t Specified_Rows() const noexcept;
    FX_SYMBOL_EXPORT int64_t Specified_Cols() const noexcept;
  FX_SYMBOL_EXPORT int64_t Data_Elements() const  noexcept;

 FX_SYMBOL_EXPORT int64_t Actual_Rows();
    FX_SYMBOL_EXPORT int64_t Specified_Rows();
  FX_SYMBOL_EXPORT int64_t Actual_Cols();
    FX_SYMBOL_EXPORT int64_t Specified_Cols();

  private:
    const fx::core::Matrix &m_;

  };
