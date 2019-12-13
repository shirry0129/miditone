#include "ClientRequests.hpp"
#include "ClientResponses.hpp"

namespace api_client {
    namespace request {
        // ############################################
        // ヘルパー関数
        // ############################################
        http::Request create_base_request(unsigned int http_version, const string_type& token) {
            const auto base_req =
                http::Request()
                .set(http_version)
                .set(http::field::content_type, "application/json; charset=utf-8")
                .set(http::field::authorization, token)
                .set(http::field::connection, "close");

            return base_req;
        }

        void write_json(const response::parser::ptree_type& ptree, string_type& str) {
            std::basic_stringstream<char_type> sstream;
            write_json(sstream, ptree, false);

            str = sstream.str();
            str.pop_back(); // remove '\n'
        }


        // ############################################
        // HealthCheck
        // ############################################
        HealthCheck::HealthCheck(const ClientBase& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::HealthCheck> HealthCheck::send() const noexcept {
            return send_helper("/api/health_check", response::parser::health_check_parser);
        }


        // ############################################
        // User
        // ############################################
        User::User(const ClientBase& client, http::verb method)
            : RequestBase(client, method) {}

        User& User::set_qrcode(const string_type& qrcode) noexcept {
            qrcode_ = qrcode;

            return *this;
        }

        result_type<response::User> User::send() const noexcept {
            string_type uri = "/api/users";
            if (!qrcode_.empty()) {
                uri += "/" + qrcode_;
            }

            return send_helper(uri, response::parser::user_parser);
        }


        // ############################################
        // Users
        // ############################################
        Users::Users(const ClientBase& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::Users> Users::send() const noexcept {
            string_type uri = "/api/users";

            return send_helper(uri, response::parser::users_parser);
        }


        // ############################################
        // Preference
        // ############################################
        Preference::Preference(
            const ClientBase& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : RequestBase(client, method), qrcode_(qrcode), platform_(platform) {}

        Preference& Preference::params(
            const std::optional<float>& note_speed,
            const std::optional<int>& se_volume
        ) {
            response::parser::ptree_type ptree;
            if (note_speed)
                ptree.put("preference.note_speed", note_speed.value());
            if (se_volume)
                ptree.put("preference.se_volume", se_volume.value());

            write_json(ptree, params_);

            return *this;
        }

        result_type<response::Preference> Preference::send() const noexcept {
            const string_type uri = "/api/users/" + qrcode_ + '/' + platform_ + '/' + "preference";

            return send_helper(uri, response::parser::preference_parser);
        }


        // ############################################
        // UsersScore
        // ############################################
        UsersScore::UsersScore(
            const ClientBase& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : RequestBase(client, method), qrcode_(qrcode), platform_(platform) {}

        UsersScore& UsersScore::difficulty(const string_type& diff) {
            url_param("difficulty", diff);

            return *this;
        }

        result_type<response::UsersScore> UsersScore::send() const noexcept {
            const string_type uri = "/api/users/" + qrcode_ + '/' + platform_ + '/' + "scores";

            return send_helper(uri, response::parser::users_score_parser);
        }


        // ############################################
        // UsersScore
        // ############################################
        NewRecord::NewRecord(
            const ClientBase& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : RequestBase(client, method), qrcode_(qrcode), platform_(platform) {}

        NewRecord& NewRecord::params(
            const new_record_params& req_params
        ) {
            response::parser::ptree_type ptree;

            ptree.put("score.music_id", req_params.music_id);
            ptree.put("score.difficulty", req_params.difficulty);
            ptree.put("score.points", req_params.points);
            ptree.put("score.max_combo", req_params.max_combo);
            ptree.put("score.critical_count", req_params.critical_count);
            ptree.put("score.correct_count", req_params.correct_count);
            ptree.put("score.nice_count", req_params.nice_count);
            ptree.put("score.miss_count", req_params.miss_count);

            write_json(ptree, params_);

            return *this;
        }

        result_type<response::UsersScore> NewRecord::send() const noexcept {
            const string_type uri = "/api/users/" + qrcode_ + '/' + platform_ + '/' + "scores/new_record";

            return send_helper(uri, response::parser::users_score_parser);
        }


        // ############################################
        // Ranking
        // ############################################
        Ranking::Ranking(
            const ClientBase& client,
            http::verb method,
            int music_id,
            const string_type& platform
        ) : RequestBase(client, method), music_id_(music_id), platform_(platform) {}

        Ranking& Ranking::difficulty(const string_type& diff) {
            url_param("difficulty", diff);

            return *this;
        }

        result_type<response::Ranking> Ranking::send() const noexcept {
            const string_type uri = "/api/musics/" + std::to_string(music_id_) + '/' + platform_ + '/' + "ranking";

            return send_helper(uri, response::parser::ranking_parser);
        }


        // ############################################
        // PlayedTimes
        // ############################################
        PlayedTimes::PlayedTimes(
            const ClientBase& client,
            http::verb method,
            int music_id,
            const string_type& platform
        ) : RequestBase(client, method), music_id_(music_id), platform_(platform) {}

        result_type<response::PlayedTimes> PlayedTimes::send() const noexcept {
            const string_type uri = "/api/musics/" + std::to_string(music_id_) + '/' + platform_ + '/' + "played_times";

            return send_helper(uri, response::parser::played_times_parser);
        }


        // ############################################
        // PlayedTimesList
        // ############################################
        PlayedTimesList::PlayedTimesList(
            const ClientBase& client,
            http::verb method,
            const string_type& platform
        ) : RequestBase(client, method), platform_(platform) {}

        result_type<response::PlayedTimesList> PlayedTimesList::send() const noexcept {
            const string_type uri = "/api/musics/" + platform_ + '/' + "played_times";

            return send_helper(uri, response::parser::played_times_list_parser);
        }


        // ############################################
        // Music
        // ############################################
        Music::Music(
            const ClientBase& client,
            http::verb method
        ) : RequestBase(client, method) {}

        Music& Music::set_id(int id) noexcept {
            id_ = id;

            return *this;
        }

        result_type<response::Music> Music::send() const noexcept {
            const string_type uri = "/api/musics/" + std::to_string(id_);

            return send_helper(uri, response::parser::music_parser);
        }


        // ############################################
        // Musics
        // ############################################
        Musics::Musics(
            const ClientBase& client,
            http::verb method
        ) : RequestBase(client, method) {}

        result_type<response::Musics> Musics::send() const noexcept {
            const string_type uri = "/api/musics/";

            return send_helper(uri, response::parser::musics_parser);
        }
    }
}
