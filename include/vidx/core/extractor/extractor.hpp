#pragma once

#include <string_view>

namespace vidx {

template <typename TResult> class Extractor {
public:
  virtual ~Extractor() = default;

  [[nodiscard]]
  virtual TResult Extract(std::string_view content) const = 0;
};

} // namespace vidx
