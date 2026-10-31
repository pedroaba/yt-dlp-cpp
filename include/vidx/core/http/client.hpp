#pragma once

#include "vidx/core/http/response.hpp"
#include <string_view>

namespace vidx {
class HttpClient {
public:
  [[nodiscard]]
  HttpResponse Get(const std::string_view &url) const;
};
} // namespace vidx
