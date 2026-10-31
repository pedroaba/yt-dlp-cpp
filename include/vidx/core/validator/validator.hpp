#pragma once

namespace vidx {
template <typename T> class Validator {
public:
  virtual ~Validator() = default;

  [[nodiscard]]
  virtual bool Validate(const T &value) const = 0;
};
} // namespace vidx
