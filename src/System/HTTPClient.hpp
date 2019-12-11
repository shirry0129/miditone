#pragma once

// reference : https://github.com/MurakamiShun/HttpClient

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>

#include "Result.hpp"


namespace http {
    using char_type = char;
    using string_type = std::basic_string<char_type>;

    using field = boost::beast::http::field;
    using verb = boost::beast::http::verb;
    using http_status = boost::beast::http::status;

    namespace version {
        static constexpr unsigned int _10 = 10;  // HTTP/1.0
        static constexpr unsigned int _11 = 11;  // HTTP/1.1
        static constexpr unsigned int _20 = 11;  // HTTP/2.0
    }


    struct BodyAccesable {
        virtual string_type& body() & noexcept = 0;
        virtual const string_type& body() const & noexcept = 0;
    };


    class ConnectionError : public BodyAccesable {
    public:
        ConnectionError(const string_type& message);

        /// <summary>
        /// 接続エラーのメッセージを返す
        /// </summary>
        string_type& body() & noexcept override;

        /// <summary>
        /// 接続エラーのメッセージを返す
        /// </summary>
        const string_type& body() const & noexcept override;

    private:
        string_type message_;
    };


    class Response : public BodyAccesable {
    public:
        Response();
        Response(boost::beast::http::response<boost::beast::http::string_body>& response);
        Response(boost::beast::http::response<boost::beast::http::string_body>&& response);

        /// <summary>
        /// HTTPレスポンスステータスコードを返す
        /// </summary>
        /// <returns>
        /// ステータスコードを表現するenum class
        /// </returns>
        http_status status() const;

        /// <summary>
        /// HTTPレスポンスステータスコードを返す
        /// </summary>
        /// <returns>
        /// ステータスコードを表す符号なし整数
        /// </returns>
        unsigned int status_code() const;

        /// <summary>
        /// レスポンスのボディを返す
        /// </summary>
        string_type& body() & noexcept override;

        /// <summary>
        /// レスポンスのボディを返す
        /// </summary>
        const string_type& body() const & noexcept override;

        /// <summary>
        /// レスポンスのヘッダを返す
        /// </summary>
        boost::beast::http::fields& header() noexcept;

        const boost::beast::http::fields& header() const noexcept;

    private:
        boost::beast::http::response<boost::beast::http::string_body> response_;
    };


    class Request : public BodyAccesable {
    public:
        Request();
        Request(verb method, const string_type& uri, unsigned int http_version = version::_11);

        Request& set(verb method, const string_type& uri, unsigned int http_version);
        Request& set(field field_name, const string_type& value);
        Request& set(unsigned int http_version);
        Request& set(verb method, const string_type& uri);
        Request& set_body(const string_type& value);

        string_type& body() & noexcept override;
        const string_type& body() const & noexcept override;

        [[nodiscard]] Result<Response, ConnectionError> send(const string_type& host, const string_type& port);

    private:
        boost::beast::http::request<boost::beast::http::string_body> request_;
    };


    using result_type = Result<Response, ConnectionError>;
}
