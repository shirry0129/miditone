//
//  Result.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Result.hpp"
#include "../../GameInfo.hpp"

namespace ui{

    Result::Result(const InitData& init):
    IScene(init),
    maxWidth(470),
    countDown(20),
    albumArt(getData().resultSongInfo.at(getData().trackCount - 1).first.jacketPath){
        for (auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
        countDown.start();
    }
    
    void Result::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const {
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
    
    void Result::update() {
        if (gameinfo::decide.pressed() || countDown.reachedZero()) {
            if (getData().trackCount < gameinfo::totalTrack) {
                changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
            }else{
                changeScene(SceneName::TOTALRESULT, gameinfo::fadeTime);
            }
        };
    }
    
    void Result::draw() const {
        ClearPrint();
        TextureAsset(U"result").drawAt(::gameinfo::originalScreenCenter);
        
        {
            Transformer2D scaler(Mat3x2::Scale(gameinfo::scale, Vec2(0, 0)));
            TextureAsset(U"name").draw(0, 0);
            TextureAsset(U"track").draw(25, 115);
            FontAsset(U"80_bold")(getData().trackCount).drawAt(296, 180, Palette::Darkslategray);
            FontAsset(U"45_bold")(getData().userName).draw(Arg::topLeft = Vec2(50, 45), gameinfo::defaultFontColor);
        }
        FontAsset(U"100_bold")(countDown.s()).draw(Arg::topRight(::gameinfo::originalResolution.x - 10, 0), gameinfo::defaultFontColor);
        FontAsset(U"100_bold")(Pad(getData().resultScore.at(getData().trackCount - 1), {7, U'0'})).draw(601, 217, gameinfo::defaultFontColor);
        drawDecision({300, 354});
        drawSongInfo({1142, 239});
        
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
    }
    
    void Result::drawDecision(const s3d::Vec2 &pos) const {
        double offset = -34 + 535;
        TextureAsset(U"resultCombo").scaled(1.3).draw(pos);
        TextureAsset(U"resultDecision").scaled(1.3).draw(pos + Vec2(-34 , TextureAsset(U"resultCombo").height()) * 1.3);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).combo).draw(Arg::topRight(pos + Vec2(506, -9) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).criticalCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 18) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).correctCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 96) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).niceCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 167) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).missCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 237) * 1.3), gameinfo::defaultFontColor);
    }
    
    void Result::drawSongInfo(const s3d::Vec2 &pos) const {
        TextureAsset(U"boxTemplate").resized(550, 600).draw(pos);
        albumArt.resized(410).draw(pos + Vec2(70, 40));
        compressedDisplay(pos + Vec2(275, 485), FontAsset(U"50_bold"), getData().resultSongInfo.at(getData().trackCount - 1).first.songInfo.title());
        compressedDisplay(pos + Vec2(275, 530), FontAsset(U"30"), getData().resultSongInfo.at(getData().trackCount - 1).first.songInfo.artist());
    }
    

}
