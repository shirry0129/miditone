#include <sstream>

#include "MiditoneClient.hpp"


namespace api_client {
    MiditoneClient::MiditoneClient() {}

    MiditoneClient::MiditoneClient(const string_type& host, const string_type& port, const string_type& token) :
        ClientBase(host, port, token) {}

    request::result_type<response::User> MiditoneClient::get_user(
        const std::string& qrcode
    ) const noexcept {
        return request::User(*this, http::verb::get).set_qrcode(qrcode).send();
    }

    CollectionRequest<request::Users> MiditoneClient::get_users(
    ) const noexcept {
        return CollectionRequest(request::Users(*this, http::verb::get));
    }

    request::result_type<response::Preference> MiditoneClient::put_button_preference(
        const string_type& qrcode,
        const std::optional<float>& note_speed,
        const std::optional<int>& se_volume
    ) const noexcept {
        return request::Preference(*this, http::verb::put, qrcode, platform::button)
               .params(note_speed, se_volume)
               .send();
    }

    request::result_type<response::Preference> MiditoneClient::put_board_preference(
        const string_type& qrcode,
        const std::optional<float>& note_speed,
        const std::optional<int>& se_volume
    ) const noexcept {
        return request::Preference(*this, http::verb::put, qrcode, platform::board)
               .params(note_speed, se_volume)
               .send();
    }

    CollectionRequest<request::UsersScore> MiditoneClient::get_users_button_score(
        const string_type& qrcode,
        const string_type& diff
    ) const noexcept {
        auto req = request::UsersScore(*this, http::verb::get, qrcode, platform::button);
        if (!diff.empty()) {
            req.difficulty(diff);
        }
        return CollectionRequest(req);
    }

    CollectionRequest<request::UsersScore> MiditoneClient::get_users_board_score(
        const string_type& qrcode,
        const string_type& diff
    ) const noexcept {
        auto req = request::UsersScore(*this, http::verb::get, qrcode, platform::board);
        if (!diff.empty()) {
            req.difficulty(diff);
        }
        return CollectionRequest(req);
    }

	request::result_type<response::UsersScore>  MiditoneClient::put_users_button_score(
        const string_type& qrcode,
        const request::new_record_params& params
    ) const noexcept {
		return request::NewRecord(*this, http::verb::put, qrcode, platform::button).params(params).send();
	}

    request::result_type<response::UsersScore> MiditoneClient::put_users_board_score(
        const string_type& qrcode,
        const request::new_record_params& params
    ) const noexcept {
        return request::NewRecord(*this, http::verb::put, qrcode, platform::board).params(params).send();
    }

    CollectionRequest<request::Musics> MiditoneClient::get_musics(
    ) const noexcept {
        return CollectionRequest(request::Musics(*this, http::verb::get));
    }

    request::result_type<response::Music> MiditoneClient::get_music(
        int id
    ) const noexcept {
        return request::Music(*this, http::verb::get).set_id(id).send();
    }

    CollectionRequest<request::Ranking> MiditoneClient::get_button_score_ranking(
        int music_id,
        const string_type& diff
    ) const noexcept {
        auto req = request::Ranking(*this, http::verb::get, music_id, platform::button);
        if (!diff.empty()) {
            req.difficulty(diff);
        }

        return CollectionRequest(req);
    }

    CollectionRequest<request::Ranking> MiditoneClient::get_board_score_ranking(
        int music_id,
        const string_type& diff
    ) const noexcept {
        auto req = request::Ranking(*this, http::verb::get, music_id, platform::board);
        if (!diff.empty()) {
            req.difficulty(diff);
        }

        return CollectionRequest(req);
    }

    request::result_type<response::PlayedTimes> MiditoneClient::get_button_played_times(
        int music_id
    )const noexcept {
        return request::PlayedTimes(*this, http::verb::get, music_id, platform::button).send();
    }

    request::result_type<response::PlayedTimes> MiditoneClient::get_board_played_times(
        int music_id
    ) const noexcept {
        return request::PlayedTimes(*this, http::verb::get, music_id, platform::board).send();
    }

    CollectionRequest<request::PlayedTimesList> MiditoneClient::get_button_played_times(
    ) const noexcept {
        return CollectionRequest(request::PlayedTimesList(*this, http::verb::get, platform::button));
    }

    CollectionRequest<request::PlayedTimesList> MiditoneClient::get_board_played_times(
    ) const noexcept {
        return CollectionRequest(request::PlayedTimesList(*this, http::verb::get, platform::board));
    }

    request::result_type<response::HealthCheck> api_client::MiditoneClient::get_health_check(
    ) const noexcept {
        return request::HealthCheck(*this, http::verb::get).send();
    }

    request::User api_client::MiditoneClient::requst_of_get_user(
    ) const noexcept {
        return request::User(*this, http::verb::get);
    }

    request::Users api_client::MiditoneClient::request_of_get_users(
    ) const noexcept {
        return request::Users(*this, http::verb::get);
    }
}

