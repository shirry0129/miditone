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
        request::result_type<response::Users> get_users() const noexcept;

        /// <summary>
        /// ボタン版の設定を更新する
        /// </summary>
        /// <param name="qrcode">対象のユーザのQRコード</param>
        /// <param name="note_speed">ノートスピード</param>
        /// <param name="se_volume">効果音の音量</param>
        request::result_type<response::Preference> put_button_preference(
            const string_type& qrcode,
            const std::optional<float>& note_speed = std::nullopt,
            const std::optional<int>& se_volume = std::nullopt
        ) const noexcept;

        /// <summary>
        /// バランスボード版の設定を更新する
        /// </summary>
        /// <param name="qrcode">対象のユーザのQRコード</param>
        /// <param name="note_speed">ノートスピード</param>
        /// <param name="se_volume">効果音の音量</param>
        request::result_type<response::Preference> put_board_preference(
            const string_type& qrcode,
            const std::optional<float>& note_speed = std::nullopt,
            const std::optional<int>& se_volume = std::nullopt
        ) const noexcept;

        /// <summary>
        /// ユーザのボタン版のスコアを全て取得する
        /// </summary>
        /// <param name="qrcode">対象のユーザのQRコード</param>
        request::result_type<response::UsersScore> get_users_button_score(
            const string_type& qrcode
        ) const noexcept;

        /// <summary>
        /// ユーザのバランスボード版のスコアを全て取得する
        /// </summary>
        /// <param name="qrcode">対象のユーザのQRコード</param>
        request::result_type<response::UsersScore> get_users_board_score(
            const string_type& qrcode
        ) const noexcept;

        /// <summary>
        /// ユーザのボタン版のスコアを更新する
        /// </summary>
        request::result_type<response::UsersScore> put_users_button_score(
            const string_type& qrcode,
            const request::new_record_params& params
        ) const noexcept;

        /// <summary>
        /// ユーザのバランスボード版のスコアを更新する
        /// </summary>
        request::result_type<response::UsersScore> put_users_board_score(
            const string_type& qrcode,
            const request::new_record_params& params
        ) const noexcept;

        /// <summary>
        /// ボタン版のスコアランキングを取得する
        /// ※ スコアの降順に返します
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        request::result_type<response::Ranking> get_button_score_ranking(
            int music_id
        ) const noexcept;

        /// <summary>
        /// バランスボード版のスコアランキングを取得する
        /// ※ スコアの降順に返します
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        request::result_type<response::Ranking> get_board_score_ranking(
            int music_id
        ) const noexcept;

        /// <summary>
        /// ボタン版の曲のプレイ回数を取得する
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        request::result_type<response::PlayedTimes> get_button_played_times(
            int music_id
        ) const noexcept;

        /// <summary>
        /// バランスボード版の曲のプレイ回数を取得する
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        request::result_type<response::PlayedTimes> get_board_played_times(
            int music_id
        ) const noexcept;

        /// <summary>
        /// ボタン版の全曲のプレイ回数を取得する
        /// ※ 曲IDの昇順に返します
        /// </summary>
        request::result_type<response::PlayedTimesList> get_button_played_times(
        ) const noexcept;

        /// <summary>
        /// バランスボード版の全曲のプレイ回数を取得する
        /// ※ 曲IDの昇順に返します
        /// </summary>
        request::result_type<response::PlayedTimesList> get_board_played_times(
        ) const noexcept;



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
