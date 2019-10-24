#include <sstream>

#include "MiditoneClient.hpp"


namespace api_client {
    MiditoneClient::MiditoneClient() {}

    MiditoneClient::MiditoneClient(const string_type& host, const string_type& port, const string_type& token) :
        host_(host), port_(port), token_(token) {}

    MiditoneClient& MiditoneClient::destination(const string_type& host, const string_type& port) {
        host_ = host;
        port_ = port;

        return *this;
    }

    connection_dest_t MiditoneClient::destination() const noexcept {
        return { host_, port_ };
    }

    MiditoneClient& MiditoneClient::token(const string_type& token) noexcept {
        token_ = token;

        return *this;
    }

    const string_type& MiditoneClient::token() const noexcept {
        return token_;
    }

    request::result_type<response::User> MiditoneClient::get_user(
        const std::string& qrcode
    ) const noexcept {
        return request::User(*this, http::verb::get).set_qrcode(qrcode).send();
    }

    request::result_type<response::Users> MiditoneClient::get_users(
    ) const noexcept {
        return request::Users(*this, http::verb::get).send();
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

    request::result_type<response::UsersScore> MiditoneClient::get_users_button_score(
        const string_type& qrcode
    ) const noexcept {
        return request::UsersScore(*this, http::verb::get, qrcode, platform::button).send();
    }

    request::result_type<response::UsersScore> MiditoneClient::get_users_board_score(
        const string_type& qrcode
    ) const noexcept {
        return request::UsersScore(*this, http::verb::get, qrcode, platform::board).send();
    }

    request::result_type<response::Ranking> MiditoneClient::get_button_score_ranking(
        int music_id
    ) const noexcept {
        return request::Ranking(*this, http::verb::get, music_id, platform::button).send();
    }

    request::result_type<response::Ranking> MiditoneClient::get_board_score_ranking(
        int music_id
    ) const noexcept {
        return request::Ranking(*this, http::verb::get, music_id, platform::board).send();
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

    request::result_type<response::PlayedTimesList> MiditoneClient::get_button_played_times(
    ) const noexcept {
        return request::PlayedTimesList(*this, http::verb::get, platform::button).send();
    }

    request::result_type<response::PlayedTimesList> MiditoneClient::get_board_played_times(
    ) const noexcept {
        return request::PlayedTimesList(*this, http::verb::get, platform::board).send();
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

