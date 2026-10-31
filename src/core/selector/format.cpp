#include "vidx/core/selector/format.hpp"

#include <stdexcept>

namespace vidx {

std::optional<MediaFormat>
FormatSelector::SelectByItag(const std::vector<MediaFormat> &formats,
                             int itag) const {
  std::optional<MediaFormat> selected;

  for (const auto &format : formats) {
    if (format.Itag != itag) {
      continue;
    }

    if (selected) {
      throw std::runtime_error("Mais de um formato possui esse itag; "
                               "será necessário outro critério de seleção.");
    }

    selected = format;
  }

  return selected;
}

} // namespace vidx