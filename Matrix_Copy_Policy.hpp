
#ifndef FX_CORE_MATRIX_COPY_POLICY_HPP
#define FX_CORE_MATRIX_COPY_POLICY_HPP

namespace fx {
namespace core {

struct shallow_copy_{};
  struct deep_copy_{};

  static shallow_copy_ shallow_copy;
  static deep_copy_ deep_copy;

} // namespace core
} // namespace fx

#endif
