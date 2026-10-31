#pragma once

#include "vidx/core/models/media_format.hpp"

#include <optional>
#include <vector>

namespace vidx {
class FormatSelector {
public:
  [[nodiscard]]
  std::optional<MediaFormat>
  SelectByItag(const std::vector<MediaFormat> &formats, int itag) const;
};
} // namespace vidx