#include "vidx/core/validator/url.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: vidx <url>\n";
    return 1;
  }

  const std::string url{argv[1]};
  const vidx::UrlValidator validator;

  if (!validator.Validate(url)) {
    std::cerr << "URL inválida. Informe um endereço HTTP ou HTTPS.\n";
    return 1;
  }

  std::cout << "URL válida: " << url << '\n';

  return 0;
}