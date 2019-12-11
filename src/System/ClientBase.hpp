#pragma once

#include <string>

namespace api_client {
    using char_type = char;
    using string_type = std::basic_string<char_type>;

    struct connection_dest_t {
        string_type host, port;
    };

    class ClientBase {
    public:
        static constexpr unsigned int http_version = http::version::_11;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        ClientBase() {};

        /// <summary>
        /// コンストラクタ
        /// 接続先情報を元に構築
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <param name="token">APIのトークン</param>
        ClientBase(const string_type& host, const string_type& port, const string_type& token) :
            host_(host), port_(port), token_(token) {}

        /// <summary>
        /// 接続先情報を登録する
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <returns>
        /// *this
        /// </returns>
        ClientBase& destination(const string_type& host, const string_type& port) {
            host_ = host;
            port_ = port;

            return *this;
        }

        /// <summary>
        /// 接続先情報を取得する
        /// </summary>
        /// <returns>
        /// 接続先の情報
        /// </returns>
        connection_dest_t destination() const noexcept {
            return { host_, port_ };
        }

        /// <summary>
        /// APIのトークンを登録する
        /// </summary>
        /// <param name="token">APIのトークン</param>
        /// <returns>
        /// *this
        /// </returns>
        ClientBase& token(const string_type& token) noexcept  {
            token_ = token;

            return *this;
        }

        /// <summary>
        /// APIのトークンを取得する
        /// </summary>
        /// <returns>
        /// APIのトークン
        /// </returns>
        const string_type& token() const noexcept {
            return token_;
        }
    private:
        string_type host_;
        string_type port_;
        string_type token_;
    };
};
