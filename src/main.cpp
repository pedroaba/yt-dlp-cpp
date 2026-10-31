#include "vidx/core/extractor/html_title.hpp"
#include "vidx/core/http/client.hpp"
#include "vidx/core/validator/url.hpp"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: vidx <url>\n";
    return 1;
  }

  const std::string url{argv[1]};
  const vidx::UrlValidator validator;
  const vidx::HttpClient client;

  if (!validator.Validate(url)) {
    std::cerr << "URL inválida. Informe um endereço HTTP ou HTTPS.\n";
    return 1;
  }

  try {
    const auto response = client.Get(url);
    const auto status = static_cast<long>(response.StatusCode);

    std::cout << "HTTP: " << static_cast<long>(response.StatusCode) << '\n';

    if (status < 200 || status >= 300) {
      std::cerr << "O servidor retornou HTTP " << status << '\n';
      return 1;
    }

    const vidx::HtmlTitleExtractor extractor;
    const auto title = extractor.Extract(response.Body);

    if (title) {
      std::cout << "Título: " << *title << '\n';
    } else {
      std::cout << "A página não possui um título disponível.\n";
    }
  } catch (const std::exception &error) {
    std::cerr << "Falha na requisição: " << error.what() << '\n';
    return 1;
  }

  return 0;
}