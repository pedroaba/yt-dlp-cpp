#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace vidx {
struct VideoInfo {
  std::string Id;
  std::string Title;
  std::string ChannelId;
  std::string ChannelName;
  std::optional<std::uint64_t> DurationSeconds;
};
} // namespace vidx
