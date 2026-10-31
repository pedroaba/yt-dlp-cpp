#pragma once

#include "vidx/core/extractor/extractor.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace vidx {

class HtmlTitleExtractor final : public Extractor<std::optional<std::string>> {
public:
  [[nodiscard]]
  std::optional<std::string> Extract(std::string_view html) const override;
};

} // namespace vidx
