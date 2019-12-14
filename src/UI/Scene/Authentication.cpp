//
//  Authentication.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/16.
//

#include "Authentication.hpp"

namespace ui{
    
    Authentication::Authentication(const InitData& init):
    IScene(init),
    cam(0),
    authed(false),
    isFailed(false),
    settings(U"../settings.ini")
    {
        getData().client.destination(settings[U"auth.host"].narrow(), settings[U"auth.port"].narrow());
        getData().client.token(settings[U"auth.token"].narrow());
        
        getData().currentDiff = Parse<size_t>(settings[U"defaultValue.difficulty"]);
        
        cam.setResolution(DisplayResolution::SVGA_800x600);
        if (!cam.start()) {
            throw Error(U"webcam not available.");
        }
        
        for (const auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
    }
    
    void Authentication::update(){
        if (gameinfo::back.down()) {
            getData().user.name = "Guest";
            getData().speed = Parse<double>(settings[U"defaultValue.speed"]);
            getData().decisionVolume = Parse<double>(settings[U"defaultValue.seVolume"]);
            changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
        }

        if (authed && gameinfo::decide.down()) {
            changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
        }
        
        if (!authed && gameinfo::prev.down()) {
            changeScene(SceneName::TITLE, gameinfo::fadeTime);
        }

        if (!cam.hasNewFrame()) {
            return ;
        }


        cam.getFrame(camImage);
        camTexture.fillIfNotBusy(camImage);
        
        if (decoder.decode(camImage, qrData)) {
            authed = true;
            cam.stop();

            const auto& getUserResult = getData().client.get_user(qrData.text.narrow());

            if (getUserResult) {
                const auto& response = getUserResult.success_value();
                if (response.status() == http::http_status::ok) {
                    getData().isGuest = false;
                    getData().user = response.parsed_body().user;

#if defined(MIDITONE_WIIBALANCEBOARD)
                    getData().speed = response.parsed_body().board_pref.note_speed.value_or(Parse<double>(settings[U"defaultValue.speed"]));
                    getData().decisionVolume = response.parsed_body().board_pref.se_volume.value_or(Parse<double>(settings[U"defaultValue.seVolume"]));
#else
                    getData().speed = response.parsed_body().button_pref.note_speed.value_or(Parse<double>(settings[U"defaultValue.speed"]));
                    getData().decisionVolume = response.parsed_body().button_pref.se_volume.value_or(Parse<double>(settings[U"defaultValue.seVolume"]));
#endif
                } else {
                    isFailed = true;
                    Logger << U"[get user] status error   : " << response.status_code();
                }
            } else {
                isFailed = true;
                const auto& error = getUserResult.failed_value();
                Logger << U"[get user] error      : " << Unicode::Widen(error.body());
            }

            if (isFailed) {
                getData().user.name = "Guest";
                getData().speed = Parse<double>(settings[U"defaultValue.speed"]);
                getData().decisionVolume = Parse<double>(settings[U"defaultValue.seVolume"]);
            }


#ifdef MIDITONE_WIIBALANCEBOARD
            const auto& getUsersScoreResult = getData().client.get_users_board_score(getData().user.qrcode).all();
#else
            const auto& getUsersScoreResult = getData().client.get_users_button_score(getData().user.qrcode).all();
#endif

            if (getUsersScoreResult) {
                const auto& response = getUsersScoreResult.success_value();
                for (const auto& record : response.parsed_body()) {
                    for (auto& score : getData().scoreList) {
                        if (score.songInfo.id() != record.music.id) {
                            continue;
                        }
                        
                        if (record.score.difficulty == "easy") {
                            score.highScore.easy.combo = record.score.max_combo;
                            score.highScore.easy.point = record.score.points;
                        }
                        if (record.score.difficulty == "normal") {
                            score.highScore.normal.combo = record.score.max_combo;
                            score.highScore.normal.point = record.score.points;
                        }
                        if (record.score.difficulty == "hard") {
                            score.highScore.hard.combo = record.score.max_combo;
                            score.highScore.hard.point = record.score.points;
                        }
                    }
                }
            } else {
                const auto& error = getUsersScoreResult.failed_value();
                Logger << U"[get users score] status error : " << Unicode::Widen(error.body());
            }

        }
    }

    void Authentication::draw() const{
        TextureAsset(U"login").drawAt(gameinfo::originalScreenCenter);
        
        if (camTexture && !authed) {
            camTexture.drawAt(gameinfo::originalScreenCenter);
        }
        
        if (authed) {
            if (isFailed) {
                const String errorMsg = U"Network Error. Continue as Guest.";
                Rect(Arg::center = gameinfo::originalScreenCenter, gameinfo::originalResolution.x, 100).draw(Color(Palette::Darkslategray, 200));
                FontAsset(U"80_bold")(errorMsg).draw(Arg::center = gameinfo::originalScreenCenter, gameinfo::defaultFontColor);
            } else {
                FontAsset(U"100_bold")(U"Welcome!").draw(Arg::center = gameinfo::originalScreenCenter - Vec2(0, 240), gameinfo::infoFontColor);
                Rect(Arg::center = gameinfo::originalScreenCenter, gameinfo::originalResolution.x, FontAsset(U"100_bold").height() * 1.75).draw(Color(Palette::Darkslategray, 200));
                FontAsset(U"100_bold")(Unicode::Widen(getData().user.name)).draw(Arg::center = gameinfo::originalScreenCenter, gameinfo::defaultFontColor);
                FontAsset(U"50_bold")(U"Continue with this data?").draw(Arg::center = gameinfo::originalScreenCenter + Vec2(0, 350), gameinfo::infoFontColor);
            }
        }
        
        for (const auto [i, rect] : Indexed(instructionBox)) {
            rect(TextureAsset(U"instBack")).draw();
            switch (i) {
                case 0:
                    if (!authed) {
                        FontAsset(U"50")(U"Cancel").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 2:
                    if (authed) {
                        FontAsset(U"50")(U"OK").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 3:
                    if (!isFailed) {
                        FontAsset(U"50")(U"Guest").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                default:
                    break;
            }
        }
    }

}
