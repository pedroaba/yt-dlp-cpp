#include "vidx/core/extractor/video_info.hpp"
#include "vidx/core/http/client.hpp"
#include "vidx/core/validator/url.hpp"

#include "vidx/core/selector/format.hpp"

#include <charconv>
#include <optional>
#include <string_view>
#include <system_error>

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2 && argc != 3) {
    std::cerr << "Uso: vidx <url> [itag]\n";
    return 1;
  }

  std::optional<int> requestedItag;

  if (argc == 3) {
    const std::string_view argument{argv[2]};
    int itag{};

    const auto [end, error] = std::from_chars(
        argument.data(), argument.data() + argument.size(), itag);

    if (error != std::errc{} || end != argument.data() + argument.size() ||
        itag <= 0) {
      std::cerr << "O itag deve ser um número inteiro positivo.\n";
      return 1;
    }

    requestedItag = itag;
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

    if (requestedItag) {
      const vidx::FormatSelector selector;

      const auto selected =
          selector.SelectByItag(video.Formats, *requestedItag);

      if (!selected) {
        std::cerr << "O formato solicitado não foi encontrado.\n";
        return 1;
      }

      std::cout << "\nFormato selecionado: " << selected->Itag
                << "\nTipo: " << selected->MimeType << '\n';

      if (selected->HasSignatureCipher) {
        std::cout << "Esse formato exige tratamento de assinatura.\n";
      }

      if (!selected->Url) {
        std::cout << "Esse formato não forneceu uma URL direta.\n";
      } else {
        std::cout << "O formato possui uma URL para a próxima etapa.\n";
      }
    }
  } catch (const std::exception &error) {
    std::cerr << "Falha ao processar vídeo: " << error.what() << '\n';
    return 1;
  }

  return 0;
}