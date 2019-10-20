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

    request::result_type<response::User> MiditoneClient::get_user(const std::string& qrcode) const noexcept {
        return request::User(*this, http::verb::get).set_qrcode(qrcode).send();
    }

    request::result_type<response::Users> MiditoneClient::get_users() const noexcept {
        return request::Users(*this, http::verb::get).send();
    }

    request::result_type<response::HealthCheck> api_client::MiditoneClient::get_health_check() const noexcept {
        return request::HealthCheck(*this, http::verb::get).send();
    }

    request::User api_client::MiditoneClient::requst_of_get_user() const noexcept {
        return request::User(*this, http::verb::get);
    }

    request::Users api_client::MiditoneClient::request_of_get_users() const noexcept {
        return request::Users(*this, http::verb::get);
    }
}

