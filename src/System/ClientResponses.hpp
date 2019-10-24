#pragma once

#include <string>
#include <functional>
#include <optional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "HTTPClient.hpp"


namespace api_client {

    using char_type = http::char_type;
    using string_type = http::string_type;
    using int_type = int;
    using float_type = float;

    class MiditoneClient;

    namespace response {
        // ###################################
        // リソース構造体
        // ###################################
        struct health_check_attr {
            string_type title;
            string_type comment;
            string_type auth;
            string_type luck;
        };
        struct preference_attr {
            std::optional<float_type> note_speed;
            std::optional<int_type> se_volume;
            // ゲームバージョン  (`button' or `board')
            string_type platform;
        };
        struct user_attr {
            // ユーザID
            int_type id;
            string_type name;
            string_type qrcode;
        };
        struct user_t {
            user_attr user;
            // ボタン版の設定
            preference_attr button_pref;
            // バランスボード版の設定
            preference_attr board_pref;
        };
        struct score_attr {
            // 曲ID
            std::optional<int_type> music_id;
            // ユーザID (QRコードではない)
            std::optional<int_type> user_id;
            // 難易度
            string_type difficulty;
            // スコア
            std::optional<int_type> points;
            std::optional<int_type> max_combo;
            std::optional<int_type> critical_count;
            std::optional<int_type> correct_count;
            std::optional<int_type> nice_count;
            std::optional<int_type> miss_count;
            // プレイ回数
            std::optional<int_type> played_times;
            // ゲームバージョン  (`button' or `board')
            string_type platform;
        };
        struct music_attr {
            // 曲ID
            int_type id;
            string_type title;
            string_type artist;
        };
        struct users_score_t {
            music_attr music;
            score_attr score;
        };
        struct musics_score_t {
            score_attr score;
            user_attr user;
        };
        using ranking_t = std::vector<musics_score_t>;
        struct played_times_attr {
            // 曲ID
            std::optional<int_type> music_id;
            // プレイ回数
            std::optional<int_type> times;
            // ゲームバージョン  (`button' or `board')
            string_type platform;
        };


        // ###################################
        // リソースのパーサー
        // ###################################
        namespace parser {
            using ptree_type = boost::property_tree::basic_ptree<string_type, string_type>;

            template <typename resource_type>
            using body_parser_t = std::function<resource_type(const ptree_type &)>;

            health_check_attr health_check_parser(const ptree_type&);
            std::vector<user_attr> users_parser(const ptree_type& ptree);
            user_t user_parser(const ptree_type& ptree);
            preference_attr preference_parser(const ptree_type& ptree);
            std::vector<users_score_t> users_score_parser(const ptree_type& ptree);
            ranking_t ranking_parser(const ptree_type& ptree);
            played_times_attr played_times_parser(const ptree_type& ptree);
            std::vector<played_times_attr> played_times_list_parser(const ptree_type& ptree);
        }


        // レスポンスクラス
        // パーサーの呼び出しを行うtemplateクラス
        template<typename T>
        struct ResponseBase : public http::Response {
            using resource_type = T;

        public:
            ResponseBase(const http::Response& response, parser::body_parser_t<resource_type> parser)
                : http::Response(response) {
                if (200 <= response.status_code() && response.status_code() <= 299) {
                    std::basic_stringstream<char_type> sstream;
                    sstream << response.body();

                    parser::ptree_type ptree;
                    read_json(sstream, ptree);

                    parsed_body_ = parser(ptree);
                }
            }

            resource_type& parsed_body() & noexcept { return parsed_body_; }
            resource_type&& parsed_body() && noexcept { return std::move(parsed_body_); }
            const resource_type& parsed_body() const & noexcept { return parsed_body_; };
            const resource_type&& parsed_body() const&& noexcept { return std::move(parsed_body_); };

        private:
            resource_type parsed_body_;
        };


        // レスポンスクラスの別名宣言
        using HealthCheck = ResponseBase<health_check_attr>;
        using User = ResponseBase<user_t>;
        using Users = ResponseBase<std::vector<user_attr>>;
        using Preference = ResponseBase<preference_attr>;
        using UsersScore = ResponseBase<std::vector<users_score_t>>;
        using Ranking = ResponseBase<ranking_t>;
        using PlayedTimes = ResponseBase<played_times_attr>;
        using PlayedTimesList = ResponseBase<std::vector<played_times_attr>>;
    }
}
