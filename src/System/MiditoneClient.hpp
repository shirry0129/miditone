#pragma once

#include <string>
#include <memory>
#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "HTTPClient.hpp"


namespace api_client {

    using char_type = http::char_type;
    using string_type = http::string_type;

    class MiditoneClient;

    namespace response {
        namespace parser {
            using ptree_type = boost::property_tree::basic_ptree<string_type, string_type>;

            template <typename resource_type>
            using body_parser_t = std::function<resource_type(const ptree_type &)>;
        }

        template<typename T>
        struct ResponseBase : public http::Response {
            using resource_type = T;

        public:
            ResponseBase(const http::Response& response, parser::body_parser_t<resource_type> parser)
                : http::Response(response) {
                    std::basic_stringstream<char_type> sstream;
                    sstream << response.body();

                    parser::ptree_type ptree;
                    read_json(sstream, ptree);

                    parsed_body_ = parser(ptree);
                }

            resource_type& parsed_body() & noexcept { return parsed_body_; }
            resource_type&& parsed_body() && noexcept { return std::move(parsed_body_); }
            const resource_type& parsed_body() const & noexcept { return parsed_body_; };
            const resource_type&& parsed_body() const&& noexcept { return std::move(parsed_body_); };

        private:
            resource_type parsed_body_;
        };


        struct health_check_t {
            string_type title;
            string_type comment;
            string_type auth;
            string_type luck;
        };
        using HealthCheck = ResponseBase<health_check_t>;

        struct user_t {
            string_type name;
            string_type qrcode;
        };
        using User = ResponseBase<user_t>;
        using Users = ResponseBase<std::vector<user_t>>;
    }


    namespace request {
        template<typename S>
        using result_type = Result<S, http::ConnectionError>;

        http::Request create_base_request(unsigned int http_version, const string_type& token);

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


    struct connection_dest_t {
        string_type host, port;
    };

    class MiditoneClient {
    public:
        static constexpr unsigned int http_version = http::version::_11;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        MiditoneClient();

        /// <summary>
        /// コンストラクタ
        /// 接続先情報を元に構築
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <param name="token">APIのトークン</param>
        MiditoneClient(const string_type& host, const string_type& port, const string_type& token);

        /// <summary>
        /// 接続先情報を登録する
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <returns>
        /// *this
        /// </returns>
        MiditoneClient& destination(const string_type& host, const string_type& port);

        /// <summary>
        /// 接続先情報を取得する
        /// </summary>
        /// <returns>
        /// 接続先の情報
        /// </returns>
        connection_dest_t destination() const noexcept;

        /// <summary>
        /// APIのトークンを登録する
        /// </summary>
        /// <param name="token">APIのトークン</param>
        /// <returns>
        /// *this
        /// </returns>
        MiditoneClient& token(const string_type& token) noexcept;

        /// <summary>
        /// APIのトークンを取得する
        /// </summary>
        /// <returns>
        /// APIのトークン
        /// </returns>
        const string_type& token() const noexcept;

        /// <summary>
        /// ユーザを取得する
        /// </summary>
        /// <param name="qrcode">取得するユーザのQRコード</param>
        request::result_type<response::User> get_user(const std::string& qrcode) const noexcept;

        /// <summary>
        /// ユーザの一覧を取得する
        /// </summary>
        /// <param name="qrcode">取得するユーザのQRコード</param>
        request::result_type<response::Users> get_users() const noexcept;

        /// <summary>
        /// サーバーの接続テストをする
        /// </summary>
        request::result_type<response::HealthCheck> get_health_check() const noexcept;

        request::User requst_of_get_user() const noexcept;
        request::Users request_of_get_users() const noexcept;

    private:
        string_type host_;
        string_type port_;
        string_type token_;
    };
}
