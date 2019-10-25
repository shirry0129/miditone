//
//  TotalResult.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/13.
//

#include "TotalResult.hpp"

using namespace ui;

TotalResult::TotalResult(const InitData& init) :
    IScene(init),
    countDown(20),
    maxWidth(340) {
    for (auto i : step(gameinfo::totalTrack)) {
        albumArt.emplace_back(getData().resultSongInfo.at(i).first.jacketPath);
    }
    for (auto i : step(4)) {
        instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
    }

    if (!getData().isGuest) {
        Array<api_client::request::new_record_params> records;
        for (size_t i = 0; i < gameinfo::totalTrack; i++) {
            api_client::request::new_record_params record;
            record.music_id         = getData().resultSongInfo.at(i).first.songInfo.id();
            record.points           = getData().resultScore.at(i);
            record.max_combo        = getData().decisionCount.at(i).combo;
            record.critical_count   = getData().decisionCount.at(i).criticalCount;
            record.correct_count    = getData().decisionCount.at(i).correctCount;
            record.nice_count       = getData().decisionCount.at(i).niceCount;
            record.miss_count       = getData().decisionCount.at(i).missCount;

            switch (getData().resultSongInfo.at(i).second) {
            case score::Difficulty::EASY:
                record.difficulty = api_client::difficulty::easy;
                break;
            case score::Difficulty::NORMAL:
                record.difficulty = api_client::difficulty::normal;
                break;
            case score::Difficulty::HARD:
                record.difficulty = api_client::difficulty::hard;
                break;
            default:
                record.difficulty = "";
            }

            records.push_back(record);
        }

#if defined(MIDITONE_WIIBALANCEBOARD)
        getData().client.put_board_preference(getData().user.qrcode, getData().speed, getData().decisionVolume);
        for (const auto& record : records) {
            getData().client.put_users_board_score(getData().user.qrcode, record);
        }
#else
        getData().client.put_button_preference(getData().user.qrcode, getData().speed, getData().decisionVolume);
        for (const auto& record : records) {
            getData().client.put_users_button_score(getData().user.qrcode, record);
        }
#endif

    }
    
    countDown.start();
}

void TotalResult::update() {
    if (gameinfo::decide.down() || countDown.reachedZero()) {
        changeScene(SceneName::GAMEOVER, gameinfo::fadeTime);
    };
}

void TotalResult::draw() const {
    TextureAsset(U"totalResult").drawAt(::gameinfo::originalScreenCenter);
    
    {
        Transformer2D scaler(Mat3x2::Scale(gameinfo::scale, Vec2(0, 0)));
        TextureAsset(U"name").draw(0, 0);
        FontAsset(U"45_bold")(Unicode::Widen(getData().user.name)).draw(Arg::topLeft = Vec2(50, 45), gameinfo::defaultFontColor);
    }
    FontAsset(U"100_bold")(countDown.s()).draw(Arg::topRight(::gameinfo::originalResolution.x - 10, 0), gameinfo::defaultFontColor);
    
    for (auto [i, rect] : Indexed(instructionBox)) {
        rect(TextureAsset(U"instBack")).draw();
        switch (i) {
            case 2:
                FontAsset(U"50")(U"Next").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                break;
            default:
                break;
        }
    }
    
    FontAsset(U"100_bold")(getData().resultScore.sum()).draw(Arg::leftCenter(953, 276), gameinfo::defaultFontColor);
    
    for (auto i : step(::gameinfo::totalTrack)) {
        drawTrack({320 + 440 * i, 350}, i);
    }

}

void TotalResult::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const {
    auto _region = assetInfo(string).region();
    if (_region.w > maxWidth) {
        Vec2 penPos(centerPos - Vec2(maxWidth / 2, _region.h / 2));
        const double charWidthLate = static_cast<double>(maxWidth) / _region.w;
        for (const auto &c : assetInfo(string)) {
            c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, gameinfo::defaultFontColor);
            penPos.x += c.xAdvance * charWidthLate;
        }
    }else{
        assetInfo(string).drawAt(centerPos, gameinfo::defaultFontColor);
    }
}

void TotalResult::drawTrack(const s3d::Vec2 &pos, size_t track) const {
    String diff;
    Color diffColor;
    
    switch (getData().resultSongInfo.at(track).second) {
        case score::Difficulty::EASY:
            diff = U"EASY";
            diffColor = gameinfo::easy;
            break;
        case score::Difficulty::NORMAL:
            diff = U"NORMAL";
            diffColor = gameinfo::normal;
            break;
        case score::Difficulty::HARD:
            diff = U"HARD";
            diffColor = gameinfo::hard;
            break;
    }
    
    TextureAsset(U"boxTemplate").resized(400, 500).draw(pos);
    FontAsset(U"30")(U"Track {}"_fmt(track + 1)).draw(pos + Vec2(20, 10), gameinfo::defaultFontColor);
    FontAsset(U"30")(diff).draw(Arg::topRight(pos + Vec2(380, 10)), diffColor);
    albumArt.at(track).resized(260).draw(pos + Vec2(70, FontAsset(U"30").height() + 10));
    compressedDisplay(pos + Vec2(200, 300 + FontAsset(U"30").height()), FontAsset(U"45_bold"), getData().resultSongInfo.at(track).first.songInfo.title());
    compressedDisplay(pos + Vec2(200, 350 + FontAsset(U"30").height()), FontAsset(U"30"), getData().resultSongInfo.at(track).first  .songInfo.artist());
    FontAsset(U"50_bold")(Pad(getData().resultScore.at(track), {7, U'0'})).drawAt(pos + Vec2(200, 310 + FontAsset(U"30").height() + FontAsset(U"30_bold").height() + FontAsset(U"30").height()), gameinfo::defaultFontColor);
}

