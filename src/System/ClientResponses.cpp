#include "ClientResponses.hpp"

#include <iostream>

namespace api_client {
    namespace response {
        namespace parser {
            ptree_type empty_tree;

            template <typename T>
            std::optional<T> get_as_optional(const ptree_type& ptree, const string_type& path) {
                try {
                    return ptree.get<T>(path);
                } catch(const std::exception& _) {
                    return std::nullopt;
                }
            }

            health_check_attr health_check_parser(const ptree_type& ptree) {
                health_check_attr parsed;
                parsed.title    = ptree.get<string_type>("title", "");
                parsed.comment  = ptree.get<string_type>("comment", "");
                parsed.auth     = ptree.get<string_type>("auth", "");
                parsed.luck     = ptree.get<string_type>("luck", "");

                return parsed;
            }

            user_attr user_attributes_parser(const ptree_type& ptree, const string_type& path_prefix = "") {
                user_attr parsed;
                parsed.name     = ptree.get<string_type>(path_prefix + "attributes.name", "");
                parsed.qrcode   = ptree.get<string_type>(path_prefix + "attributes.qrcode", "");

                return parsed;
            }

            preference_attr preference_attributes_parser(const ptree_type& ptree, const string_type& path_prefix = "") {
                preference_attr parsed;
                parsed.note_speed   = get_as_optional<float_type>(ptree, path_prefix + "attributes.note_speed");
                parsed.se_volume    = get_as_optional<int_type>(ptree, path_prefix + "attributes.se_volume");
                parsed.platform     = ptree.get<string_type>(path_prefix + "attributes.platform", "");

                return parsed;
            }

            user_t user_parser(const ptree_type& ptree) {
                user_t parsed;
                parsed.user = user_attributes_parser(ptree, "data.");

                for (const auto& included : ptree.get_child("included", empty_tree)) {
                    const string_type type = included.second.get<string_type>("type", "");
                    if (type == "preference") {
                        const string_type platform = included.second.get<string_type>("attributes.platform", "");
                        if (platform == "button")
                            parsed.button_pref = preference_attributes_parser(included.second);
                        else if (platform == "board")
                            parsed.board_pref = preference_attributes_parser(included.second);
                    }
                }

                return parsed;
            }

            std::vector<user_attr> users_parser(const ptree_type& ptree) {
                std::vector<user_attr> parsed;

                for (const auto & data_tree : ptree.get_child("data", empty_tree)) {
                    parsed.emplace_back(user_attributes_parser(data_tree.second));
                }

                return parsed;
            }
        }
    }
}
