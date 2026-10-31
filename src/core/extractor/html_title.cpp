#include "vidx/core/extractor/html_title.hpp"

#include <cstddef>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

namespace vidx {
std::optional<std::string>
HtmlTitleExtractor::Extract(std::string_view html) const {
  if (html.empty()) {
    return std::nullopt;
  }

  if (html.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw std::length_error("HTML excede o tamanho suportado.");
  }

  const std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> document(
      htmlReadMemory(
          html.data(), static_cast<int>(html.size()), nullptr, nullptr,
          HTML_PARSE_NONET | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING),
      xmlFreeDoc);

  if (!document) {
    throw std::runtime_error("Não foi possível interpretar o HTML recebido.");
  }

  const std::unique_ptr<xmlXPathContext, decltype(&xmlXPathFreeContext)>
      context(xmlXPathNewContext(document.get()), xmlXPathFreeContext);

  if (!context) {
    throw std::runtime_error("Não foi possível criar o contexto XPath.");
  }

  const auto *expression = reinterpret_cast<const xmlChar *>(
      "normalize-space(string(/html/head/title[1]))");

  const std::unique_ptr<xmlXPathObject, decltype(&xmlXPathFreeObject)> result(
      xmlXPathEvalExpression(expression, context.get()), xmlXPathFreeObject);

  if (!result || !result->stringval) {
    throw std::runtime_error("Não foi possível consultar o título.");
  }

  std::string title(reinterpret_cast<const char *>(result->stringval));
  if (title.empty()) {
    return std::nullopt;
  }

  return title;
}
} // namespace vidx