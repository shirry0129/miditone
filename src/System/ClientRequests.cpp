#include "ClientRequests.hpp"

#include "MiditoneClient.hpp"
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
        HealthCheck::HealthCheck(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::HealthCheck> HealthCheck::send() const noexcept {
            const auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, "/api/health_check")
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::HealthCheck>(response::HealthCheck(result.success_value(), response::parser::health_check_parser));
            else
                return result_type<response::HealthCheck>(result.failed_value());
        }


        // ############################################
        // User
        // ############################################
        User::User(const MiditoneClient& client, http::verb method)
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

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::User>(response::User(result.success_value(), response::parser::user_parser));
            else
                return result_type<response::User>(result.failed_value());
        }


        // ############################################
        // Users
        // ############################################
        Users::Users(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::Users> Users::send() const noexcept {
            string_type uri = "/api/users";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::Users>(response::Users(result.success_value(), response::parser::users_parser));
            else
                return result_type<response::Users>(result.failed_value());
        }


        // ############################################
        // Preference
        // ############################################
        Preference::Preference(
            const MiditoneClient& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : qrcode_(qrcode), platform_(platform), RequestBase(client, method) {}

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

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .set_body(params_)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::Preference>(response::Preference(result.success_value(), response::parser::preference_parser));
            else
                return result_type<response::Preference>(result.failed_value());
        }


        // ############################################
        // UsersScore
        // ############################################
        UsersScore::UsersScore(
            const MiditoneClient& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : qrcode_(qrcode), platform_(platform), RequestBase(client, method) {}

        result_type<response::UsersScore> UsersScore::send() const noexcept {
            const string_type uri = "/api/users/" + qrcode_ + '/' + platform_ + '/' + "scores";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::UsersScore>(response::UsersScore(result.success_value(), response::parser::users_score_parser));
            else
                return result_type<response::UsersScore>(result.failed_value());
        }


        // ############################################
        // UsersScore
        // ############################################
        NewRecord::NewRecord(
            const MiditoneClient& client,
            http::verb method,
            const string_type& qrcode,
            const string_type& platform
        ) : qrcode_(qrcode), platform_(platform), RequestBase(client, method) {}

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

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .set_body(params_)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::UsersScore>(response::UsersScore(result.success_value(), response::parser::users_score_parser));
            else
                return result_type<response::UsersScore>(result.failed_value());
        }


        // ############################################
        // Ranking
        // ############################################
        Ranking::Ranking(
            const MiditoneClient& client,
            http::verb method,
            int music_id,
            const string_type& platform
        ) : music_id_(music_id), platform_(platform), RequestBase(client, method) {}

        result_type<response::Ranking> Ranking::send() const noexcept {
            const string_type uri = "/api/musics/" + std::to_string(music_id_) + '/' + platform_ + '/' + "ranking";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::Ranking>(response::Ranking(result.success_value(), response::parser::ranking_parser));
            else
                return result_type<response::Ranking>(result.failed_value());
        }


        // ############################################
        // PlayedTimes
        // ############################################
        PlayedTimes::PlayedTimes(
            const MiditoneClient& client,
            http::verb method,
            int music_id,
            const string_type& platform
        ) : music_id_(music_id), platform_(platform), RequestBase(client, method) {}

        result_type<response::PlayedTimes> PlayedTimes::send() const noexcept {
            const string_type uri = "/api/musics/" + std::to_string(music_id_) + '/' + platform_ + '/' + "played_times";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::PlayedTimes>(response::PlayedTimes(result.success_value(), response::parser::played_times_parser));
            else
                return result_type<response::PlayedTimes>(result.failed_value());
        }


        // ############################################
        // PlayedTimesList
        // ############################################
        PlayedTimesList::PlayedTimesList(
            const MiditoneClient& client,
            http::verb method,
            const string_type& platform
        ) : platform_(platform), RequestBase(client, method) {}

        result_type<response::PlayedTimesList> PlayedTimesList::send() const noexcept {
            const string_type uri = "/api/musics/" + platform_ + '/' + "played_times";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::PlayedTimesList>(response::PlayedTimesList(result.success_value(), response::parser::played_times_list_parser));
            else
                return result_type<response::PlayedTimesList>(result.failed_value());
        }
    }
}
