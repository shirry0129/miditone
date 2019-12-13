#pragma once

#include "ClientRequests.hpp"
#include "ClientResponses.hpp"
#include "ClientBase.hpp"

namespace api_client {

    template<typename request_type>
    class CollectionRequest {
        using response_type = typename request_type::response_type;

    public:
        CollectionRequest(request_type&& req) : req_(req) {}
        CollectionRequest(request_type& req) : req_(req) {}

        // 最初のページを取得
        [[nodiscard]] request::result_type<response_type> first() {
            return page(1);
        }

        // ページ番号を指定して送信
        // params:
        //   page: ページ番号 (1以上の数)
        [[nodiscard]] request::result_type<response_type> page(int page_num) {
            return req_.page(page_num).send();
        }

        // 全ページを一括で取得
        [[nodiscard]] request::result_type<response_type> all() {
            auto first_result = first();
            if (!first_result)
                return first_result;

            auto& response = first_result.success_value();
            const int total_pages = response.pagination().total_pages;
            auto& collection = response.parsed_body();

            collection.reserve(response.pagination().total_records);

            for (int page_num = 2; page_num <= total_pages; page_num++) {
                auto r = page(page_num);
                if (!r)
                    break;

                const auto& body = r.success_value().parsed_body();
                collection.insert(collection.end(), body.cbegin(), body.cend());
            }

            return first_result;
        }
    private:
        request_type req_;
    };


    class MiditoneClient : public ClientBase {
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
        /// ユーザを取得する
        /// </summary>
        /// <param name="qrcode">取得するユーザのQRコード</param>
        request::result_type<response::User> get_user(const std::string& qrcode) const noexcept;

        /// <summary>
        /// ユーザの一覧を取得する
        /// </summary>
        CollectionRequest<request::Users> get_users(
        ) const noexcept;

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
        CollectionRequest<request::UsersScore> get_users_button_score(
            const string_type& qrcode,
            const string_type& diff = ""
        ) const noexcept;

        /// <summary>
        /// ユーザのバランスボード版のスコアを全て取得する
        /// </summary>
        /// <param name="qrcode">対象のユーザのQRコード</param>
        CollectionRequest<request::UsersScore> get_users_board_score(
            const string_type& qrcode,
            const string_type& diff = ""
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
        /// 曲をすべて取得する
        /// </summary>
        CollectionRequest<request::Musics> get_musics(
        ) const noexcept;

        /// <summary>
        /// 曲を1つ取得する
        /// </summary>
        /// <param name="id">取得する曲ID</param>
        request::result_type<response::Music> get_music(
            int id
        ) const noexcept;

        /// <summary>
        /// ボタン版のスコアランキングを取得する
        /// ※ スコアの降順に返します
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        CollectionRequest<request::Ranking> get_button_score_ranking(
            int music_id,
            const string_type& diff = ""
        ) const noexcept;

        /// <summary>
        /// バランスボード版のスコアランキングを取得する
        /// ※ スコアの降順に返します
        /// </summary>
        /// <param name="music_id">対象曲の曲ID</param>
        CollectionRequest<request::Ranking> get_board_score_ranking(
            int music_id,
            const string_type& diff = ""
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
        CollectionRequest<request::PlayedTimesList> get_button_played_times(
        ) const noexcept;

        /// <summary>
        /// バランスボード版の全曲のプレイ回数を取得する
        /// ※ 曲IDの昇順に返します
        /// </summary>
        CollectionRequest<request::PlayedTimesList> get_board_played_times(
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
