#include "ClientResponses.hpp"


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
                parsed.id       = std::stoi(ptree.get<string_type>(path_prefix + "id", ""));
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

            preference_attr preference_parser(const ptree_type& ptree) {
                return preference_attributes_parser(ptree, "data.");
            }

            score_attr score_attributes_parser(const ptree_type& ptree, const string_type& path_prefix = "") {
                score_attr parsed;
                parsed.music_id         = get_as_optional<int_type>(ptree, path_prefix + "attributes.music_id");
                parsed.user_id          = get_as_optional<int_type>(ptree, path_prefix + "attributes.user_id");
                parsed.difficulty       = ptree.get<string_type>(path_prefix + "attributes.difficulty", "");
                parsed.points           = get_as_optional<int_type>(ptree, path_prefix + "attributes.points");
                parsed.max_combo        = get_as_optional<int_type>(ptree, path_prefix + "attributes.max_combo");
                parsed.critical_count   = get_as_optional<int_type>(ptree, path_prefix + "attributes.critical_count");
                parsed.correct_count    = get_as_optional<int_type>(ptree, path_prefix + "attributes.correct_count");
                parsed.nice_count       = get_as_optional<int_type>(ptree, path_prefix + "attributes.nice_count");
                parsed.miss_count       = get_as_optional<int_type>(ptree, path_prefix + "attributes.miss_count");
                parsed.played_times     = get_as_optional<int_type>(ptree, path_prefix + "attributes.played_times");
                parsed.platform         = ptree.get<string_type>(path_prefix + "attributes.platform", "");

                return parsed;
            }

            music_attr music_attributes_parser(const ptree_type& ptree, const string_type& path_prefix = "") {
                music_attr parsed;
                parsed.id       = std::stoi(ptree.get<string_type>(path_prefix + "id", ""));
                parsed.title    = ptree.get<string_type>(path_prefix + "attributes.title", "");
                parsed.artist   = ptree.get<string_type>(path_prefix + "attributes.artist", "");

                return parsed;
            }

            std::vector<users_score_t> users_score_parser(const ptree_type& ptree) {
                std::vector<users_score_t> parsed;

                for (const auto& data_tree : ptree.get_child("data", empty_tree)) {
                    users_score_t us;
                    us.score = score_attributes_parser(data_tree.second);
                    parsed.emplace_back(us);
                }

                std::vector<music_attr> included_musics;
                for (const auto& included_tree : ptree.get_child("included", empty_tree)) {
                    const string_type type = included_tree.second.get<string_type>("type", "");
                    if (type == "music") {
                        included_musics.emplace_back(music_attributes_parser(included_tree.second));
                    }
                }

                // set music attributes in users_score_t structure from included musics
                for (auto& us : parsed) {
                    auto it = std::find_if(
                        included_musics.cbegin(),
                        included_musics.cend(),
                        [us] (const music_attr& music) { return music.id == us.score.music_id; }
                    );

                    if (it != included_musics.cend()) {
                        us.music = *it;
                    }
                }

                return parsed;
            }

            ranking_t ranking_parser(const ptree_type& ptree) {
                ranking_t parsed;

                for (const auto& data_tree : ptree.get_child("data", empty_tree)) {
                    musics_score_t ms;
                    ms.score = score_attributes_parser(data_tree.second);
                    parsed.emplace_back(ms);
                }

                std::vector<user_attr> included_users;
                for (const auto& included_tree : ptree.get_child("included", empty_tree)) {
                    const string_type type = included_tree.second.get<string_type>("type", "");
                    if (type == "user") {
                        included_users.emplace_back(user_attributes_parser(included_tree.second));
                    }
                }

                // set user attributes in ranking_t structure from included users
                for (auto& ms : parsed) {
                    auto it = std::find_if(
                        included_users.cbegin(),
                        included_users.cend(),
                        [ms](const user_attr& user) { return user.id == ms.score.user_id; }
                    );

                    if (it != included_users.cend()) {
                        ms.user = *it;
                    }
                }

                return parsed;
            }

            played_times_attr played_times_attributes_parser(const ptree_type& ptree, const string_type& path_prefix = "") {
                played_times_attr parsed;
                parsed.music_id = get_as_optional<int_type>(ptree, path_prefix + "attributes.music_id");
                parsed.times = get_as_optional<int_type>(ptree, path_prefix + "attributes.times");
                parsed.platform = ptree.get<string_type>(path_prefix + "attributes.platform", "");

                return parsed;
            }

            played_times_attr played_times_parser(const ptree_type& ptree)
            {
                return played_times_attributes_parser(ptree, "data.");
            }

            std::vector<played_times_attr> played_times_list_parser(const ptree_type& ptree) {
                std::vector<played_times_attr> parsed;

                for (const auto& data_tree : ptree.get_child("data", empty_tree)) {                    
                    parsed.emplace_back(played_times_attributes_parser(data_tree.second));
                }

                return parsed;
            }

            
        }
    }
}
