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
            const auto score = getData().resultScore.at(getData().trackCount - 1);
            const auto combo = getData().decisionCount.at(getData().trackCount - 1).combo;
            
            switch (getData().currentDiff) {
                case 0:
                    if (getData().currentMusic->highScore.easy.point < score) {
                        getData().currentMusic->highScore.easy.point = score;
                    }
                    if (getData().currentMusic->highScore.easy.combo < combo) {
                        getData().currentMusic->highScore.easy.combo = combo;
                    }
                    break;
                    
                case 1:
                    if (getData().currentMusic->highScore.normal.point < score) {
                        getData().currentMusic->highScore.normal.point = score;
                    }
                    if (getData().currentMusic->highScore.normal.combo < combo) {
                        getData().currentMusic->highScore.normal.combo = combo;
                    }
                    break;
                    
                case 2:
                    if (getData().currentMusic->highScore.hard.point < score) {
                        getData().currentMusic->highScore.hard.point = score;
                    }
                    if (getData().currentMusic->highScore.hard.combo < combo) {
                        getData().currentMusic->highScore.hard.combo = combo;
                    }
                    break;
                    
                default:
                    break;
            }
            
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
        
        const auto& point = getData().resultScore.at(getData().trackCount - 1);
        size_t highPoint = 0;
        
        switch (getData().currentDiff) {
            case 0:
                highPoint = getData().currentMusic->highScore.easy.point;
                break;
                
            case 1:
                highPoint = getData().currentMusic->highScore.normal.point;
                break;
                
            case 2:
                highPoint = getData().currentMusic->highScore.hard.point;
                break;
                
            default:
                break;
        }
        
        {
            Transformer2D scaler(Mat3x2::Scale(gameinfo::scale, Vec2(0, 0)));
            TextureAsset(U"name").draw(0, 0);
            TextureAsset(U"track").draw(25, 115);
            FontAsset(U"80_bold")(getData().trackCount).drawAt(296, 180, Palette::Darkslategray);
            FontAsset(U"45_bold")(Unicode::Widen(getData().user.name)).draw(Arg::topLeft = Vec2(50, 45), gameinfo::defaultFontColor);
        }
        FontAsset(U"100_bold")(countDown.s()).draw(Arg::topRight(::gameinfo::originalResolution.x - 10, 0), gameinfo::defaultFontColor);
        if (point > highPoint && !getData().isGuest) {
            FontAsset(U"45_bold")(U"+{}"_fmt(point - highPoint)).draw(Arg::topRight(1053, 197), Palette::Lightyellow);
        }
        FontAsset(U"100_bold")(U"{:0>7}"_fmt(getData().resultScore.at(getData().trackCount - 1))).draw(Arg::topRight(1053, 217), gameinfo::defaultFontColor);
        drawDecision({300, 354});
        drawSongInfo({1142, 229});
        
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
        
        const auto& combo = getData().decisionCount.at(getData().trackCount - 1).combo;
        size_t highCombo = 0;
        
        switch (getData().currentDiff) {
            case 0:
                highCombo = getData().currentMusic->highScore.easy.combo;
                break;
            case 1:
                highCombo = getData().currentMusic->highScore.normal.combo;
                break;
            case 2:
                highCombo = getData().currentMusic->highScore.hard.combo;
                break;
        }
        
        
        TextureAsset(U"resultCombo").scaled(1.3).draw(pos);
        TextureAsset(U"resultDecision").scaled(1.3).draw(pos + Vec2(-34 , TextureAsset(U"resultCombo").height()) * 1.3);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).combo).draw(Arg::topRight(pos + Vec2(506, -9) * 1.3), gameinfo::defaultFontColor);
        if (combo > highCombo && !getData().isGuest) {
            FontAsset(U"30")(U"+{}"_fmt(combo - highCombo)).draw(Arg::topRight(pos + Vec2(506, -20) * 1.3), Palette::Lightyellow);
        }
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).criticalCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 18) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).correctCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 96) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).niceCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 167) * 1.3), gameinfo::defaultFontColor);
        FontAsset(U"70")(getData().decisionCount.at(getData().trackCount - 1).missCount).draw(Arg::topRight(pos + Vec2(offset, TextureAsset(U"resultCombo").height() + 237) * 1.3), gameinfo::defaultFontColor);
    }
    
    void Result::drawSongInfo(const s3d::Vec2 &pos) const {
        String difficulty;
        Color diffColor;
        
        switch (getData().currentDiff) {
            case 0:
                difficulty = U"EASY";
                diffColor = gameinfo::easy;
                break;
            case 1:
                difficulty = U"NORMAL";
                diffColor = gameinfo::normal;
                break;
            case 2:
                difficulty = U"HARD";
                diffColor = gameinfo::hard;
                break;
            default:
                break;
        }
        
        TextureAsset(U"boxTemplate").resized(550, 620).draw(pos);
        FontAsset(U"50_bold")(difficulty).drawAt(pos + Vec2(275, 70), diffColor);
        albumArt.resized(380).drawAt(pos + Vec2(275, 290));
        compressedDisplay(pos + Vec2(275, 515), FontAsset(U"50_bold"), getData().resultSongInfo.at(getData().trackCount - 1).first.songInfo.title());
        compressedDisplay(pos + Vec2(275, 560), FontAsset(U"30"), getData().resultSongInfo.at(getData().trackCount - 1).first.songInfo.artist());
    }

}
