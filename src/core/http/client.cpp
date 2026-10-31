#include "vidx/core/http/client.hpp"
#include "vidx/core/constants/http_flags.hpp"
#include "vidx/core/http/response.hpp"
#include "vidx/core/validator/url.hpp"

#include <curl/curl.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

namespace vidx {
namespace {
void CheckCurl(CURLcode result) {
  if (result != CURLE_OK) {
    throw std::runtime_error(curl_easy_strerror(result));
  }
}

// Inicializa o libcurl e libera seus recursos ao encerrar o programa.
class CurlRuntime {
public:
  CurlRuntime() { CheckCurl(curl_global_init(CURL_GLOBAL_ALL)); }

  ~CurlRuntime() { curl_global_cleanup(); }
};

// O libcurl chama esta função para cada bloco recebido.
std::size_t WriteBody(char *data, std::size_t size, std::size_t count,
                      void *userData) noexcept {
  const auto bytes = size * count;
  auto &body = *static_cast<std::string *>(userData);

  try {
    body.append(data, bytes);
    return bytes;
  } catch (...) {
    // Uma exceção de C++ não deve escapar para o código C do libcurl.
    return 0;
  }
}
} // namespace

HttpResponse HttpClient::Get(const std::string_view &url) const {
  static const CurlRuntime runtime;

  const UrlValidator validator;
  if (!validator.Validate(url)) {
    throw std::invalid_argument("A URL contém um caractere nulo.");
  }

  const std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> handle(
      curl_easy_init(), curl_easy_cleanup);

  if (!handle) {
    throw std::runtime_error("Não foi possível criar a conexão HTTP.");
  }

  // string_view não garante um terminador nulo; std::string garante.
  const std::string address(url);
  HttpResponse response = HttpResponse{};

  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_FOLLOWLOCATION,
                             ENABLE_FOLLOW_REDIRECTS));
  CheckCurl(
      curl_easy_setopt(handle.get(), CURLOPT_MAXREDIRS, MAX_ALLOWED_REDIRECTS));

  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_URL, address.c_str()));
  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, &WriteBody));
  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &response.Body));
  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_CONNECTTIMEOUT,
                             MAX_CONNECTION_TIMEOUT));
  CheckCurl(curl_easy_setopt(handle.get(), CURLOPT_TIMEOUT, MAX_TIMEOUT));
  CheckCurl(curl_easy_perform(handle.get()));

  char *effectiveUrl = nullptr;
  CheckCurl(
      curl_easy_getinfo(handle.get(), CURLINFO_EFFECTIVE_URL, &effectiveUrl));

  if (effectiveUrl != nullptr) {
    response.EffectiveUrl = effectiveUrl;
  }

  long statusCode{};
  CheckCurl(
      curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &statusCode));

  response.StatusCode = static_cast<HttpStatusCode>(statusCode);
  return response;
}
} // namespace vidx
