
#ifndef FX_CORE_DIMENSION_TYPE_HPP
#define  FX_CORE_DIMENSION_TYPE_HPP

#include <cstdint>
#include <initializer_list>
#include <utility>
#include <exception>
#include <stdexcept>

namespace fx  {
  namespace core {

struct Dimension_Type {
  int64_t rows, cols;
  Dimension_Type() : rows(1), cols(1) {}
  Dimension_Type(int rows_, int cols_) : rows(rows_), cols(cols_) {}
  Dimension_Type(std::pair<int64_t, int64_t> dims) : rows(dims.first), cols(dims.second) {}
  Dimension_Type(std::initializer_list<int64_t> vals) {
    //      #ifndef _MSC_VER
    //   static_assert(vals.size() == 2, "Dimension_Type can only be initialized with a 2-element initializer_list");
    //#else
      if (vals.size() != 2)
          throw std::runtime_error("Dimension_Type can only be initialized with a 2-element initializer_list");
      //#endif
      auto it = vals.begin();
      rows = *it; ++it;
      cols = *it;
  }
//  Dimension_Type(std::tuple<int64_t, int64_t> dims) : rows(std::get<0>(dims)), cols(std::get<1>(dims)) {}

  template <typename Archive>
  void serialize(Archive &ar, const unsigned int version) {
      ar & rows & cols;
  }
};

    namespace internal {
      struct Matrix_Size_Type {
	int64_t major_dim, minor_dim;
      };
    bool operator== (fx::core::internal::Matrix_Size_Type const& a, fx::core::internal::Matrix_Size_Type const& b);
        bool operator!= (fx::core::internal::Matrix_Size_Type const& a, fx::core::internal::Matrix_Size_Type const& b);

      struct Utility_Functions{
      template <typename M>
      static fx::core::Dimension_Type MajorMinor_To_RowsCols(M m_, fx::core::internal::Matrix_Size_Type dim);

      };
      
    }

bool operator== (fx::core::Dimension_Type const& a, fx::core::Dimension_Type const& b);
  bool operator!= (fx::core::Dimension_Type const& a, fx::core::Dimension_Type const& b);
  } // namespace core
  } // namespace fx





#endif
