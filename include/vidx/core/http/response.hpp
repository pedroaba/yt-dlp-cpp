#pragma once

#include "vidx/core/http/status_code.hpp"
#include <string>

namespace vidx {
struct HttpResponse {
  vidx::HttpStatusCode
      StatusCode{}; // Zero means no HTTP response received yet.
  std::string body;
};
} // namespace vidx
