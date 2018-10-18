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
    
    void MusicBox::compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Rect &_region, const s3d::Font &assetInfo, const s3d::String &string) const {
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
        auto titleRegion(FontAsset(U"songTitle")(musicInfo.title()).region());
        auto artistRegion(FontAsset(U"musicInfo")(musicInfo.artist()).region());
        
        entity.movedBy(moveWidth).scaled(scale)(TextureAsset(U"boxTemplate")).draw();
        albumArt.resized(320, 320).scaled(scale).drawAt(entity.center() + moveWidth + Vec2(0, -100 * scale));
        for (auto d : step(diffBox.size())) {
            Color diffColor, diffFrame;
            
            switch (d) {
                case 0:
                    diffColor = Palette::Limegreen;
                    diffFrame = Color(U"#259925");
                    break;
                case 1:
                    diffColor = Palette::Darkorange;
                    diffFrame = Color(U"#b36200");
                    break;
                case 2:
                    diffColor = Palette::Crimson;
                    diffFrame = Color(U"#b31031");
                    break;
                default:
                    break;
            }
            
            diffBox.at(d).movedBy(moveWidth + Vec2((-95 + (int)d * 95), 230) * scale).scaled(scale).draw(diffColor).drawFrame(4, 0, diffFrame);
        }
        
        Transformer2D t(Mat3x2::Scale(scale, titleCenter));
        compressedDisplay(titleCenter, titleRegion, FontAsset(U"songTitle"), musicInfo.title());
        t = Transformer2D(Mat3x2::Scale(scale, artistCenter));
        compressedDisplay(artistCenter, artistRegion, FontAsset(U"musicInfo"), musicInfo.artist());
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
    defaultEntity(Arg::center(Window::Center()), boxSize) {
        getData().trackCount++;
        for (auto file : getData().scoreList) {
            music.emplace_back(file, defaultEntity);
        }
    }
    
    void MusicSelect::update() {
        if (gameinfo::backArrow.down()) {
            if (getData().currentMusic > 0) {
                getData().currentMusic--;
            }
        }
        if (gameinfo::goArrow.down()) {
            if (getData().currentMusic < music.size() - 1) {
                getData().currentMusic++;
            }
        }
        if (gameinfo::decide.down()) {
            getData().scoreFile = music.at(getData().currentMusic).getScoreFile();
            getData().musicFile = U"../Score/music/{}.mp3"_fmt(music.at(getData().currentMusic).getMusicInfo().id());
            changeScene(SceneName::PREFERENCE, gameinfo::fadeTime);
        };
    }
    
    void MusicSelect::draw() const {
        ClearPrint();
        
        TextureAsset(U"select").drawAt(Window::Center());
        
        for (auto i : step(music.size())) {
            music.at(i).draw(Vec2(((int)i - (int)getData().currentMusic) * defaultEntity.w, 0));
        }
    }

}
