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
    albumArt(U"../Score/albumArt/{}.png"_fmt(getData().resultSongInfo.at(getData().trackCount - 1).id)){
        for (auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
    }
    
    void Result::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const {
        auto _region = assetInfo(string).region();
        
        if (_region.w > maxWidth) {
            Vec2 penPos(centerPos - Vec2(maxWidth / 2, _region.h / 2));
            const double charWidthLate = static_cast<double>(maxWidth) / _region.w;
            for (const auto &c : assetInfo(string)) {
                c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, gameinfo::fontColor);
                penPos.x += c.xAdvance * charWidthLate;
            }
        }else{
            assetInfo(string).drawAt(centerPos, gameinfo::fontColor);
        }
    }
    
    void Result::update() {
        if (button.pressed()) {
            if (getData().trackCount < gameinfo::totalTrack) {
                changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
            }else{
                changeScene(SceneName::TOTALRESULT, gameinfo::fadeTime);
            }
        };
    }
    
    void Result::draw() const {
        ClearPrint();
        TextureAsset(U"result").drawAt(Window::Center());
        TextureAsset(U"track").draw(0, 0);
        FontAsset(U"trackFont")(getData().trackCount).drawAt(273, 66, Palette::Darkslategray);
        FontAsset(U"resultScore")(Pad(getData().resultScore.at(getData().trackCount - 1), {7, U'0'})).draw(601, 217, gameinfo::fontColor);
        drawDecision({300, 354});
        drawSongInfo({1142, 239});
        
        for (auto [i, rect] : Indexed(instructionBox)) {
            switch (i) {
                case 0:
                    rect(TextureAsset(U"instBack")).draw();
                    break;
                case 1:
                    rect(TextureAsset(U"instBack")).draw();
                    break;
                case 2:
                    rect(TextureAsset(U"instBack")).draw();
                    FontAsset(U"infoFont")(U"Next").drawAt(rect.center() + Vec2(0, 25), Color(U"#061e38"));
                    break;
                case 3:
                    rect(TextureAsset(U"instBack")).draw();
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
        FontAsset(U"decisionFont")(getData().decisionCount.at(getData().trackCount - 1).combo).draw(Arg::topRight(pos + Vec2(506, -9) * 1.3), gameinfo::fontColor);
        FontAsset(U"decisionFont")(getData().decisionCount.at(getData().trackCount - 1).criticalCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 18) * 1.3), gameinfo::fontColor);
        FontAsset(U"decisionFont")(getData().decisionCount.at(getData().trackCount - 1).correctCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 96) * 1.3), gameinfo::fontColor);
        FontAsset(U"decisionFont")(getData().decisionCount.at(getData().trackCount - 1).niceCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 167) * 1.3), gameinfo::fontColor);
        FontAsset(U"decisionFont")(getData().decisionCount.at(getData().trackCount - 1).missCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 237) * 1.3), gameinfo::fontColor);
    }
    
    void Result::drawSongInfo(const s3d::Vec2 &pos) const {
        TextureAsset(U"boxTemplate").resized(550, 600).draw(pos);
        albumArt.resized(410).draw(pos + Vec2(70, 40));
        compressedDisplay(pos + Vec2(275, 485), FontAsset(U"songTitle"), getData().resultSongInfo.at(getData().trackCount - 1).title);
        compressedDisplay(pos + Vec2(275, 530), FontAsset(U"musicInfo"), getData().resultSongInfo.at(getData().trackCount - 1).artist);
    }
    

}
