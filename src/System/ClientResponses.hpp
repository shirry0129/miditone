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
        // リソース構造体
        struct health_check_attr {
            string_type title;
            string_type comment;
            string_type auth;
            string_type luck;
        };
        struct preference_attr {
            std::optional<float_type> note_speed;
            std::optional<int_type> se_volume;
            // `button' or `board'
            string_type platform;
        };
        struct user_attr {
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


        // リソースのパーサー
        namespace parser {
            using ptree_type = boost::property_tree::basic_ptree<string_type, string_type>;

            template <typename resource_type>
            using body_parser_t = std::function<resource_type(const ptree_type &)>;

            health_check_attr health_check_parser(const ptree_type&);
            std::vector<user_attr> users_parser(const ptree_type& ptree);
            user_t user_parser(const ptree_type& ptree);

        }

        // レスポンスクラス
        // パーサーの呼び出しを行うtemplateクラス
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


        // レスポンスクラスの特殊化
        using HealthCheck = ResponseBase<health_check_attr>;
        using User = ResponseBase<user_t>;
        using Users = ResponseBase<std::vector<user_attr>>;
    }
}
