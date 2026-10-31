#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace vidx {
struct MediaFormat {
  int Itag{};
  std::string MimeType;
  std::string QualityLabel;

  std::optional<int> Height;
  std::optional<std::uint64_t> Bitrate;
  std::optional<std::string> Url;

  bool IsAdaptive{};
  bool HasSignatureCipher{};
};
} // namespace vidx