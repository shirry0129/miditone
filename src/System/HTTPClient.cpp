#include "HTTPClient.hpp"


namespace http {

    ConnectionError::ConnectionError(const string_type& message) : message_(message) {}

    string_type& ConnectionError::body() & noexcept {
        return message_;
    }

    const string_type& ConnectionError::body() const & noexcept {
        return message_;
    }

    Response::Response() {};

    Response::Response(boost::beast::http::response<boost::beast::http::string_body>& response) :
        response_(response) {};

    Response::Response(boost::beast::http::response<boost::beast::http::string_body>&& response) :
        response_(response) {};

    http_status Response::status() const {
        return response_.result();
    }

    unsigned int Response::status_code() const {
        return response_.result_int();
    }

    string_type& Response::body() & noexcept {
        return response_.body();
    }

    const string_type& Response::body() const & noexcept {
        return response_.body();
    }

    boost::beast::http::fields& Response::header() noexcept {
        return response_.base();
    }

    const boost::beast::http::fields& Response::header() const noexcept {
        return response_.base();
    }

    Request::Request() {}

    Request::Request(verb method, const string_type& uri, unsigned int http_version) :
        request_(method, uri, http_version) {}

    Request& Request::set(verb method, const string_type& uri, unsigned int http_version) {
        request_.method(method);
        request_.target(uri);
        request_.version(http_version);

        return *this;
    }

    Request& Request::set(field field_name, const string_type& value) {
        request_.set(field_name, value);

        return *this;
    }

    Request& Request::set(unsigned int http_version) {
        request_.version(http_version);

        return *this;
    }

    Request& Request::set(verb method, const string_type& uri) {
        request_.method(method);
        request_.target(uri);

        return *this;
    }

    Request& Request::set_body(const string_type& value) {
        request_.body() = value;
        set(field::content_length, std::to_string(value.size()));

        return *this;
    }

    string_type& Request::body() & noexcept {
        return request_.body();
    }

    const string_type& Request::body() const & noexcept {
        return request_.body();
    }

    Result<Response, ConnectionError> Request::send(const string_type& host, const string_type& port) {
        namespace beast = boost::beast;
        namespace http = beast::http;
        using tcp = boost::asio::ip::tcp;

        try {
            // name resolution
            boost::asio::io_context ioc;
            tcp::resolver resolver(ioc);
            auto result = resolver.resolve(host, port);
            beast::tcp_stream stream(ioc);
            stream.connect(result);

            // send request
            http::write(stream, request_);

            // get response
            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            beast::error_code error;
            stream.socket().shutdown(tcp::socket::shutdown_both, error);

            if (error && error != beast::errc::not_connected)
                throw beast::system_error{ error };

            return result_type(Response(res));
        } catch (std::exception const& e) {
            return result_type(ConnectionError(e.what()));
        }
    }
}
