#pragma once

#include <map>

#include "ClientResponses.hpp"
#include "ClientBase.hpp"

namespace api_client {
    using char_type = http::char_type;
    using string_type = http::string_type;


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
        template<typename T>
        struct RequestBase {
            using response_type = T;
        public:
            RequestBase(const ClientBase& client, http::verb method)
                : client_(client), method_(method) {};

            virtual result_type<response_type> send() const noexcept = 0;

            RequestBase& page(int page_num) noexcept {
                url_parameter_.insert_or_assign("page", std::to_string(page_num));

                return *this;
            }

        protected:
            result_type<response_type> send_helper(string_type uri, response::parser::body_parser_t<typename response_type::resource_type> parser) const noexcept {
                if (!url_parameter_.empty()) {
                    uri += "?";
                    for (const auto& param : url_parameter_) {
                        uri += param.first + "=" + param.second + "&";
                    }
                    uri.pop_back();
                }

                const auto result =
                    create_base_request(client_.http_version, client_.token())
                    .set(method_, uri)
                    .send(client_.destination().host, client_.destination().port);

                if (result)
                    return result_type<response_type>(response_type(result.success_value(), parser));
                else
                    return result_type<response_type>(result.failed_value());
            }

            const ClientBase& client_;
            http::verb method_;
            std::map<string_type, string_type> url_parameter_;
        };

        // --------------------------------------------------
        // リクエストクラスの実装
        // --------------------------------------------------

        struct HealthCheck : public RequestBase<response::HealthCheck> {
            HealthCheck(const ClientBase& client, http::verb method);

            result_type<response::HealthCheck> send() const noexcept override;
        };

        struct User : public RequestBase<response::User> {
            User(const ClientBase& client, http::verb method);

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
            Users(const ClientBase& client, http::verb method);

            result_type<response::Users> send() const noexcept override;
        };

        struct Preference : public RequestBase<response::Preference> {
            Preference(
                const ClientBase& client,
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
            string_type qrcode_;
            string_type platform_;

            string_type params_;
        };

        struct UsersScore : public RequestBase<response::UsersScore> {
            UsersScore(
                const ClientBase& client,
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
                const ClientBase& client,
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
                const ClientBase& client,
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
                const ClientBase& client,
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
                const ClientBase& client,
                http::verb method,
                const string_type& platform
            );

            result_type<response::PlayedTimesList> send() const noexcept override;

        private:
            string_type platform_;
        };
    }
}
