#include "vidx/core/extractor/html_title.hpp"
#include "vidx/core/extractor/video_info.hpp"
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

    const vidx::VideoInfoExtractor video_info_extractor;
    const auto video = video_info_extractor.Extract(response.Body);

    std::cout << "ID: " << video.Id << '\n';
    std::cout << "Título: " << video.Title << '\n';
    std::cout << "Canal: " << video.ChannelName << '\n';
    std::cout << "ID do canal: " << video.ChannelId << '\n';

    if (video.DurationSeconds) {
      std::cout << "Duração: " << *video.DurationSeconds << " segundos\n";
    } else {
      std::cout << "Duração indisponível\n";
    }

    std::cout << "\nFormatos encontrados: " << video.Formats.size() << '\n';

    for (const auto &format : video.Formats) {
      std::cout << "itag=" << format.Itag << " | " << format.MimeType
                << " | grupo="
                << (format.IsAdaptive ? "adaptiveFormats" : "formats");

      if (!format.QualityLabel.empty()) {
        std::cout << " | " << format.QualityLabel;
      }

      if (format.Height) {
        std::cout << " | altura=" << *format.Height;
      }

      if (format.Bitrate) {
        std::cout << " | bitrate=" << *format.Bitrate << " bit/s";
      }

      std::cout << " | URL presente=" << (format.Url ? "sim" : "não")
                << " | signatureCipher="
                << (format.HasSignatureCipher ? "sim" : "não") << '\n';
    }

    if (video.Formats.empty()) {
      std::cout << "Esta resposta do player não forneceu formatos listáveis.\n";
    }
  } catch (const std::exception &error) {
    std::cerr << "Falha na requisição: " << error.what() << '\n';
    return 1;
  }

  return 0;
}