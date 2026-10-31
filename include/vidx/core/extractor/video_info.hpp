#include "vidx/core/extractor/extractor.hpp"
#include "vidx/core/models/video_info.hpp"
#include <string_view>

namespace vidx {
class VideoInfoExtractor final : public Extractor<VideoInfo> {
public:
  [[nodiscard]]
  VideoInfo Extract(std::string_view html) const override;
};
} // namespace vidx
