#include "ClientRequests.hpp"

#include "MiditoneClient.hpp"
#include "ClientResponses.hpp"

namespace api_client {
    namespace request {
        http::Request create_base_request(unsigned int http_version, const string_type& token) {
            const auto base_req =
                http::Request()
                .set(http_version)
                .set(http::field::content_type, "application/json; charset=utf-8")
                .set(http::field::authorization, token)
                .set(http::field::connection, "close");

            return base_req;
        }

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
                .set(http::verb::get, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::User>(response::User(result.success_value(), response::parser::user_parser));
            else
                return result_type<response::User>(result.failed_value());
        }

        Users::Users(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::Users> Users::send() const noexcept {
            string_type uri = "/api/users";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(http::verb::get, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::Users>(response::Users(result.success_value(), response::parser::users_parser));
            else
                return result_type<response::Users>(result.failed_value());
        }
    }
}
