#pragma once

#include "vidx/core/validator/validator.hpp"
#include <string_view>

namespace vidx {
class UrlValidator final : public Validator<std::string_view> {
public:
  [[nodiscard]]
  bool Validate(const std::string_view &value) const override;
};
} // namespace vidx
