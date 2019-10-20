#pragma once

#include "ClientRequests.hpp"
#include "ClientResponses.hpp"


namespace api_client {
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
