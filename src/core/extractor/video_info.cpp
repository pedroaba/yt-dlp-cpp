#include "vidx/core/extractor/video_info.hpp"
#include "vidx/core/models/media_format.hpp"
#include "vidx/core/models/video_info.hpp"

#include <nlohmann/json.hpp>

#include <charconv>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

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

  const auto streaming = player.find("streamingData");
  if (streaming != player.end() && streaming->is_object()) {
    const auto appendFormats = [&](const char *groupName, bool adaptive) {
      const auto group = streaming->find(groupName);

      if (group == streaming->end() || !group->is_array()) {
        return;
      }

      for (const auto &entry : *group) {
        if (!entry.is_object()) {
          continue;
        }

        MediaFormat format;
        format.Itag = entry.at("itag").get<int>();
        format.MimeType = entry.at("mimeType").get<std::string>();
        format.IsAdaptive = adaptive;

        if (entry.contains("qualityLabel") &&
            !entry.at("qualityLabel").is_null()) {
          format.QualityLabel = entry.at("qualityLabel").get<std::string>();
        }

        if (entry.contains("height") && !entry.at("height").is_null()) {
          format.Height = entry.at("height").get<int>();
        }

        if (entry.contains("bitrate") && !entry.at("bitrate").is_null()) {
          format.Bitrate = entry.at("bitrate").get<std::uint64_t>();
        }

        if (entry.contains("url") && !entry.at("url").is_null()) {
          const auto url = entry.at("url").get<std::string>();

          if (!url.empty()) {
            format.Url = url;
          }
        }

        format.HasSignatureCipher =
            entry.contains("signatureCipher") &&
            entry.at("signatureCipher").is_string() &&
            !entry.at("signatureCipher").get_ref<const std::string &>().empty();

        video.Formats.push_back(format);
      }
    };

    appendFormats("formats", false);
    appendFormats("adaptiveFormats", true);
  }

  return video;
}
} // namespace vidx
