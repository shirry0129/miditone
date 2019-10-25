#pragma once

#include "ClientResponses.hpp"

namespace api_client {
    using char_type = http::char_type;
    using string_type = http::string_type;

    class MiditoneClient;


    namespace platform {
        static const string_type button = "button";
        static const string_type board = "board";
    }

    namespace difficulty {
        static const string_type easy = "easy";
        static const string_type normal = "normal";
        static const string_type hard = "hard";
    }


    namespace request {
        template<typename S>
        using result_type = Result<S, http::ConnectionError>;

        http::Request create_base_request(unsigned int http_version, const string_type& token);
        void write_json(const response::parser::ptree_type& ptree, string_type& str);

        // リクエストの基底クラス
        template<typename response_type>
        struct RequestBase {
        public:
            RequestBase(const MiditoneClient& client, http::verb method)
                : client_(client), method_(method) {};

            virtual result_type<response_type> send() const noexcept {
                return result_type<response_type>();
            }

        protected:
            const MiditoneClient& client_;
            http::verb method_;
        };


        // --------------------------------------------------
        // リクエストクラスの実装
        // --------------------------------------------------

        struct HealthCheck : public RequestBase<response::HealthCheck> {
            HealthCheck(const MiditoneClient& client, http::verb method);

            result_type<response::HealthCheck> send() const noexcept override;
        };

        struct User : public RequestBase<response::User> {
            User(const MiditoneClient& client, http::verb method);

            /// <summary>
            /// URIにQRコードを指定する
            /// </summary>
            /// <param name="qrcode">QRコードの文字列</param>
            /// <returns>
            /// *this
            /// </returns>
            User& set_qrcode(const string_type& qrcode) noexcept;

            result_type<response::User> send() const noexcept override;
        private:
            string_type qrcode_;
        };

        struct Users : public RequestBase<response::Users> {
            Users(const MiditoneClient& client, http::verb method);

            result_type<response::Users> send() const noexcept override;
        };

        struct Preference : public RequestBase<response::Preference> {
            Preference(
                const MiditoneClient& client,
                http::verb method,
                const string_type& qrcode,
                const string_type& platform
            );

            Preference& params(
                const std::optional<float>& note_speed = std::nullopt,
                const std::optional<int>& se_volume = std::nullopt
            );

            result_type<response::Preference> send() const noexcept override;
        private:
            string_type platform_;
            string_type qrcode_;

            string_type params_;
        };

        struct UsersScore : public RequestBase<response::UsersScore> {
            UsersScore(
                const MiditoneClient& client,
                http::verb method,
                const string_type& qrcode,
                const string_type& platform
            );

            result_type<response::UsersScore> send() const noexcept override;

        private:
            string_type qrcode_;
            string_type platform_;
        };

        struct new_record_params {
            int music_id;
            string_type difficulty;
            int points;
            int max_combo;
            int critical_count;
            int correct_count;
            int nice_count;
            int miss_count;
        };

        struct NewRecord : public RequestBase<response::UsersScore> {
            NewRecord(
                const MiditoneClient& client,
                http::verb method,
                const string_type& qrcode,
                const string_type& platform
            );

            NewRecord& params(
                const new_record_params& req_params
            );

            result_type<response::UsersScore> send() const noexcept override;

        private:
            string_type qrcode_;
            string_type platform_;
            string_type params_;
        };

        struct Ranking : public RequestBase<response::Ranking> {
            Ranking(
                const MiditoneClient& client,
                http::verb method,
                int music_id,
                const string_type& platform
            );

            result_type<response::Ranking> send() const noexcept override;

        private:
            int music_id_;
            string_type platform_;
        };

        struct PlayedTimes : public RequestBase<response::PlayedTimes> {
            PlayedTimes(
                const MiditoneClient& client,
                http::verb method,
                int music_id,
                const string_type& platform
            );

            result_type<response::PlayedTimes> send() const noexcept override;

        private:
            int music_id_;
            string_type platform_;
        };

        struct PlayedTimesList : public RequestBase<response::PlayedTimesList> {
            PlayedTimesList(
                const MiditoneClient& client,
                http::verb method,
                const string_type& platform
            );

            result_type<response::PlayedTimesList> send() const noexcept override;

        private:
            string_type platform_;
        };
    }
}
