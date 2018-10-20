//
//  MusicSelect.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "MusicSelect.hpp"

namespace ui{
    
    MusicBox::MusicBox(const FilePath& _info, const s3d::RectF &_entity):
    IBox(_entity, TextureAsset(U"boxTemplate")),
    maxWidth(340),
    scoreFile(_info),
    musicInfo(_info.toUTF32()){
        albumArt = Texture(U"../Score/albumArt/{}.png"_fmt(musicInfo.id()));
        for (auto i : step(3)) {
            diffBox.emplace_back(Arg::center(Window::Center()),90);
        }
    }
    
    void MusicBox::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const {
        auto _region = assetInfo(string).region();
        
        if (_region.w > maxWidth) {
            Vec2 penPos(centerPos - Vec2(maxWidth / 2, _region.h / 2));
            const double charWidthLate = 340. / _region.w;
            for (const auto &c : assetInfo(string)) {
                c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, gameinfo::fontColor);
                penPos.x += c.xAdvance * charWidthLate;
            }
        }else{
            assetInfo(string).drawAt(centerPos, gameinfo::fontColor);
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
        
        for (auto [i, lev] : Indexed(musicInfo.level())) {
            Transformer2D t(Mat3x2::Scale(scale, moveWidth + diffBox.at(i).center() + Vec2((-95 + (int)i * 95), 230) * scale));
            FontAsset(U"diffInfo")(lev).drawAt(moveWidth + diffBox.at(i).center() + Vec2((-95 + (int)i * 95), 230) * scale, Color(U"#061e38"));
        }
        
        Transformer2D t(Mat3x2::Scale(scale, titleCenter));
        compressedDisplay(titleCenter, FontAsset(U"songTitle"), musicInfo.title());
        t = Transformer2D(Mat3x2::Scale(scale, artistCenter));
        compressedDisplay(artistCenter, FontAsset(U"musicInfo"), musicInfo.artist());
    }
    
    score::Header MusicBox::getMusicInfo() const {
        return musicInfo;
    }
    
    s3d::FilePath MusicBox::getScoreFile() const {
        return scoreFile;
    }
    
    
    


    MusicSelect::MusicSelect(const InitData& init):
    IScene(init),
    boxSize(400, 600),
    defaultEntity(Arg::center(Window::Center()), boxSize),
    countDown(90){
        for (auto file : getData().scoreList) {
            music.emplace_back(file, defaultEntity);
        }
        for (auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
        
        example = Audio(U"../Score/musicEx/{}.mp3"_fmt(music.at(getData().currentMusic).getMusicInfo().id()), Arg::loop = true);
        example.play();
        countDown.start();
    }
    
    void MusicSelect::update() {
        if (gameinfo::backArrow.down()) {
            if (getData().currentMusic > 0) {
                getData().currentMusic--;
                resetEx();
            }
        }
        if (gameinfo::goArrow.down()) {
            if (getData().currentMusic < music.size() - 1) {
                getData().currentMusic++;
                resetEx();
            }
        }
        if (gameinfo::decide.down() || countDown.reachedZero()) {
            getData().scoreFile = music.at(getData().currentMusic).getScoreFile();
            getData().musicFile = U"../Score/music/{}.mp3"_fmt(music.at(getData().currentMusic).getMusicInfo().id());
            changeScene(SceneName::PREFERENCE, gameinfo::fadeTime);
        };
    }
    
    void MusicSelect::draw() const {
        ClearPrint();
        
        TextureAsset(U"select").drawAt(Window::Center());
        TextureAsset(U"track").draw(0, 0);
        FontAsset(U"trackFont")(getData().trackCount + 1).drawAt(273, 66, Palette::Darkslategray);
        FontAsset(U"countDown")(countDown.s()).draw(Arg::topRight(Window::Width() - 10, 0), gameinfo::fontColor);
        
        for (auto i : step(music.size())) {
            music.at(i).draw(Vec2(((int)i - (int)getData().currentMusic) * defaultEntity.w, 0));
        }
        
        for (auto [i, rect] : Indexed(instructionBox)) {
            switch (i) {
                case 0:
                    rect(TextureAsset(U"instBack")).draw();
                    FontAsset(U"infoFont")(U"Prev").drawAt(rect.center() + Vec2(0, 25), Color(U"#061e38"));
                    break;
                case 1:
                    rect(TextureAsset(U"instBack")).draw();
                    FontAsset(U"infoFont")(U"Next").drawAt(rect.center() + Vec2(0, 25), Color(U"#061e38"));
                    break;
                case 2:
                    rect(TextureAsset(U"instBack")).draw();
                    FontAsset(U"infoFont")(U"Select").drawAt(rect.center() + Vec2(0, 25), Color(U"#061e38"));
                    break;
                case 3:
                    rect(TextureAsset(U"instBack")).draw();
                    break;
                default:
                    break;
            }
        }
    }
    
    void MusicSelect::resetEx() {
        example.stop();
        example.release();
        example = Audio(U"../Score/musicEx/{}.mp3"_fmt(music.at(getData().currentMusic).getMusicInfo().id()), Arg::loop = true);
        example.play();
    }
    

}
