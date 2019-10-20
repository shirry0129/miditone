//
//  TotalResult.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/13.
//

#include "TotalResult.hpp"

using namespace ui;

TotalResult::TotalResult(const InitData &init):
IScene(init),
countDown(20),
maxWidth(340){
    for (auto i : step(gameinfo::totalTrack)) {
        albumArt.emplace_back(getData().resultSongInfo.at(i).first.jacketPath);
    }
    for (auto i : step(4)) {
        instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
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
    compressedDisplay(pos + Vec2(200, 300 + FontAsset(U"30").height()), FontAsset(U"totalSong"), getData().resultSongInfo.at(track).first.songInfo.title());
    compressedDisplay(pos + Vec2(200, 350 + FontAsset(U"30").height()), FontAsset(U"totalArtist"), getData().resultSongInfo.at(track).first  .songInfo.artist());
    FontAsset(U"50_bold")(Pad(getData().resultScore.at(track), {7, U'0'})).drawAt(pos + Vec2(200, 310 + FontAsset(U"30").height() + FontAsset(U"30_bold").height() + FontAsset(U"30").height()), gameinfo::defaultFontColor);
}

