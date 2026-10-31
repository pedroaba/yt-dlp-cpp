#include "vidx/core/extractor/video_info.hpp"
#include "vidx/core/models/video_info.hpp"

#include <nlohmann/json.hpp>

#include <charconv>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

namespace vidx {
VideoInfo VideoInfoExtractor::Extract(std::string_view html) const {
  // Localiza a atribuição; o parser JSON interpreta seu conteúdo.
  static const std::regex assignment(R"(\bytInitialPlayerResponse\s*=\s*)");
  std::match_results<std::string_view::const_iterator> match;

  if (!std::regex_search(html.begin(), html.end(), match, assignment)) {
    throw std::runtime_error(
        "Os dados do player não foram encontrados na página.");
  }

  // A leitura por stream consome um valor JSON, deixando o JS restante.
  std::istringstream input{std::string(match[0].second, html.end())};
  nlohmann::json player;
  input >> player;

  if (!player.contains("videoDetails") ||
      !player.at("videoDetails").is_object()) {
    throw std::runtime_error("O player não forneceu os metadados do vídeo.");
  }

  const auto &details = player.at("videoDetails");

  VideoInfo video;
  video.Id = details.at("videoId").get<std::string>();
  video.Title = details.at("title").get<std::string>();
  video.ChannelId = details.at("channelId").get<std::string>();
  video.ChannelName = details.at("author").get<std::string>();

  if (video.Id.empty() || video.Title.empty()) {
    throw std::runtime_error("Os metadados do vídeo estão incompletos.");
  }

  if (details.contains("lengthSeconds") &&
      !details.at("lengthSeconds").is_null()) {
    const auto text = details.at("lengthSeconds").get<std::string>();

    std::uint64_t seconds{};
    const auto [end, error] =
        std::from_chars(text.data(), text.data() + text.size(), seconds);

    if (error != std::errc{} || end != text.data() + text.size()) {
      throw std::runtime_error("A duração do vídeo é inválida.");
    }

    video.DurationSeconds = seconds;
  }

  return video;
}
} // namespace vidx
