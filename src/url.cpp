#include "vidx/core/validator/url.hpp"

#include <cctype>
#include <curl/curl.h>

namespace vidx {

bool UrlValidator::Validate(const std::string_view &value) const {
  return !value.empty() && value.starts_with("https://") &&
         value.find('\0') == std::string_view::npos;
}

} // namespace vidx
