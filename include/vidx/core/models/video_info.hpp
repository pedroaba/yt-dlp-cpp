#pragma once

#include "vidx/core/models/media_format.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace vidx {
struct VideoInfo {
  std::string Id;
  std::string Title;
  std::string ChannelId;
  std::string ChannelName;
  std::optional<std::uint64_t> DurationSeconds;
  std::vector<MediaFormat> Formats;
};
} // namespace vidx
