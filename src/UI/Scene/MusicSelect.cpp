//
//  MusicSelect.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "MusicSelect.hpp"

namespace ui{
    
    MusicBox::MusicBox(const ScoreData& _info, const s3d::RectF &_entity):
    IBox(_entity, TextureAsset(U"boxTemplate")),
    albumArt(_info.jacketPath),
    title(_info.songInfo.title()),
    artist(_info.songInfo.artist()),
    maxWidth(340){
        for (const auto& lev : _info.songInfo.level()){
            level.emplace_back(lev);
        }
        
        for (auto i : step(3)) {
            diffBox.emplace_back(Arg::center(::gameinfo::originalScreenCenter), 90);
        }
    }
    
    void MusicBox::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const {
        auto _region = assetInfo(string).region();
        
        if (_region.w > maxWidth) {
            Vec2 penPos(centerPos - Vec2(maxWidth / 2, _region.h / 2));
            const double charWidthLate = 340. / _region.w;
            for (const auto &c : assetInfo(string)) {
                c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, gameinfo::defaultFontColor);
                penPos.x += c.xAdvance * charWidthLate;
            }
        }else{
            assetInfo(string).drawAt(centerPos, gameinfo::defaultFontColor);
        }
    }
    
    void MusicBox::draw(const s3d::Vec2 &moveWidth) const{
        double scale = 0.8;
        
        if (moveWidth.x == 0) {
            scale = 1;
        }
        
        Vec2 titleCenter(entity.center() + moveWidth + Vec2(0, 100 * scale));
        Vec2 artistCenter(entity.center() + moveWidth + Vec2(0, 160 * scale));

        entity.movedBy(moveWidth).scaled(scale)(TextureAsset(U"boxTemplate")).draw();
        albumArt.resized(320).scaled(scale).drawAt(entity.center() + moveWidth + Vec2(0, -100 * scale));
        for (auto d : step(diffBox.size())) {
            Color diffColor, diffFrame;
            
            switch (d) {
                case 0:
                    diffColor = gameinfo::easy;
                    diffFrame = Color(U"#259925");
                    break;
                case 1:
                    diffColor = gameinfo::normal;
                    diffFrame = Color(U"#b36200");
                    break;
                case 2:
                    diffColor = gameinfo::hard;
                    diffFrame = Color(U"#b31031");
                    break;
                default:
                    break;
            }
            
            diffBox.at(d).movedBy(moveWidth + Vec2((-95 + (int)d * 95), 230) * scale).scaled(scale).draw(diffColor).drawFrame(4, 0, diffFrame);
        }
        
        for (auto [i, lev] : Indexed(level)) {
            Transformer2D t(Mat3x2::Scale(scale, moveWidth + diffBox.at(i).center() + Vec2((-95 + (int)i * 95), 230) * scale));
            FontAsset(U"45_bold")(lev).drawAt(moveWidth + diffBox.at(i).center() + Vec2((-95 + (int)i * 95), 230) * scale, Color(U"#061e38"));
        }
        
        Transformer2D t(Mat3x2::Scale(scale, titleCenter));
        compressedDisplay(titleCenter, FontAsset(U"50_bold"), title);
        t = Transformer2D(Mat3x2::Scale(scale, artistCenter));
        compressedDisplay(artistCenter, FontAsset(U"30"), artist);
    }
    
    


    MusicSelect::MusicSelect(const InitData& init):
    IScene(init),
    boxSize(400, 600),
    defaultEntity(Arg::center(::gameinfo::originalScreenCenter), boxSize),
    example(getData().currentMusic->musicPath){
        for (const auto& score : getData().scoreList) {
            musics.emplace_back(score, defaultEntity);
        }
        for (auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
        
        example.setLoop(
                        Arg::loopBegin = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusBegSec()),
                        Arg::loopEnd   = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusEndSec())
                        );
        example.setPosSec(getData().currentMusic->songInfo.chorusBegSec());
        example.play();
        
        if (!getData().isSelecting) {
            getData().isSelecting = true;
            getData().selectTimer.set(120s);
        }
        
        getData().selectTimer.start();
    }
    
    void MusicSelect::update() {
        if (gameinfo::prev.down()) {
            if (getData().currentMusic > getData().scoreList.cbegin()) {
                getData().currentMusic--;
                resetEx();
            } else if (getData().currentMusic == getData().scoreList.cbegin()) {
                getData().currentMusic = getData().scoreList.end() - 1;
                resetEx();
            }
        }
        if (gameinfo::next.down()) {
            if (getData().currentMusic < getData().scoreList.cend() - 1) {
                getData().currentMusic++;
                resetEx();
            } else if (getData().currentMusic == getData().scoreList.cend() - 1) {
                getData().currentMusic = getData().scoreList.begin();
                resetEx();
            }
        }
        if (gameinfo::decide.down()) {
            getData().selectTimer.pause();
            changeScene(SceneName::PREFERENCE, gameinfo::fadeTime);
        };
        if (getData().selectTimer.reachedZero()) {
            getData().selectTimer.pause();
            changeScene(SceneName::PLAY, gameinfo::fadeTime);
        }
    }
    
    void MusicSelect::draw() const {
        ClearPrint();
        
        TextureAsset(U"select").drawAt(::gameinfo::originalScreenCenter);

        {
            Transformer2D scaler(Mat3x2::Scale(gameinfo::scale, Vec2(0,0)));
            TextureAsset(U"name").draw(0, 0);
            TextureAsset(U"track").draw(25, 115);
            FontAsset(U"80_bold")(getData().trackCount + 1).drawAt(296, 180, Palette::Darkslategray);
            FontAsset(U"45_bold")(Unicode::Widen(getData().user.name)).draw(Arg::topLeft = Vec2(50, 45), gameinfo::defaultFontColor);
        }
        
        FontAsset(U"100_bold")(getData().selectTimer.s()).draw(Arg::topRight(::gameinfo::originalResolution.x - 10, 0), gameinfo::defaultFontColor);
        
        for (const auto i : step(musics.size())) {
            auto dist = std::distance(getData().scoreList.begin(), getData().currentMusic);
            musics.at(i).draw(
                Vec2(((int)i - dist) * defaultEntity.w, -50)
            );
        }
        
        if (!getData().isGuest) {
            drawHighScore({350, 740});
        }
        
        for (auto [i, rect] : Indexed(instructionBox)) {
            rect(TextureAsset(U"instBack")).draw();
            switch (i) {
                case 0:
                    FontAsset(U"50")(U"Prev").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    break;
                case 1:
                    FontAsset(U"50")(U"Next").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    break;
                case 2:
                    FontAsset(U"50")(U"Select").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    break;
                default:
                    break;
            }
        }
    }
    
    void MusicSelect::resetEx() {
        example.stop();
        example.release();
        example = Audio(getData().currentMusic->musicPath);
        example.setLoop(
                        Arg::loopBegin = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusBegSec()),
                        Arg::loopEnd   = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusEndSec())
                        );
        example.setPosSec(getData().currentMusic->songInfo.chorusBegSec());
        example.play();
    }

    void MusicSelect::drawHighScore(Vec2 tlPos) const {
        String easyScore = U"---";
        String normalScore = U"---";
        String hardScore = U"---";
        
        if (getData().currentMusic->highScore.easy.point.has_value()) {
            easyScore = U"{:0>7}"_fmt(getData().currentMusic->highScore.easy.point.value());
        }
        
        if (getData().currentMusic->highScore.normal.point.has_value()) {
            normalScore = U"{:0>7}"_fmt(getData().currentMusic->highScore.normal.point.value());
        }
        
        if (getData().currentMusic->highScore.hard.point.has_value()) {
            hardScore = U"{:0>7}"_fmt(getData().currentMusic->highScore.hard.point.value());
        }
                
        Rect(tlPos.asPoint(), 350, 200).draw(ColorF(Palette::Dimgray, 0.7)).drawFrame(2, 3, Palette::Gold);
        FontAsset(U"30_bold")(U"HighScore").draw(tlPos + Vec2(10, 10), gameinfo::defaultFontColor);
        FontAsset(U"30_bold")(U"EASY").draw(tlPos + Vec2(10, 60), gameinfo::easy);
        FontAsset(U"30_bold")(easyScore).draw(tlPos + Vec2(200, 60));
        FontAsset(U"30_bold")(U"NORMAL").draw(tlPos + Vec2(10, 105), gameinfo::normal);
        FontAsset(U"30_bold")(normalScore).draw(tlPos + Vec2(200, 105));
        FontAsset(U"30_bold")(U"HARD").draw(tlPos + Vec2(10, 150), gameinfo::hard);
        FontAsset(U"30_bold")(hardScore).draw(tlPos + Vec2(200, 150));
    }

}
