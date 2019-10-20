#pragma once

#include "ClientResponses.hpp"

namespace api_client {
    using char_type = http::char_type;
    using string_type = http::string_type;

    class MiditoneClient;

    namespace request {
        template<typename S>
        using result_type = Result<S, http::ConnectionError>;

        http::Request create_base_request(unsigned int http_version, const string_type& token);

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
    }
}
