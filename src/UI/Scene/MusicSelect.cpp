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
    scoreFile(_info),
    musicInfo(_info.toUTF32()){}
    
    void MusicBox::draw(const s3d::Vec2 &moveWidth) const{
        double scale = 0.8;
        
        if (moveWidth.x == 0) {
            scale = 1;
        }
        
        Vec2 titleCenter(entity.center() + moveWidth + Vec2(0, -150 * scale));
        Rect titleRegion(FontAsset(U"songTitle")(musicInfo.title()).region());
        
        entity.movedBy(moveWidth).scaled(scale)(TextureAsset(U"boxTemplate")).draw();
        Transformer2D t(Mat3x2::Scale(scale, titleCenter));
        if (titleRegion.w > 340) {
            Vec2 penPos(titleCenter - Vec2(170, titleRegion.h / 2));
            const double charWidthLate = 340. / titleRegion.w;
            for (const auto &c : FontAsset(U"songTitle")(musicInfo.title())) {
                c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, gameinfo::fontColor);
                penPos.x += c.xAdvance * charWidthLate;
            }
        }else{
            FontAsset(U"songTitle")(musicInfo.title()).drawAt(titleCenter, gameinfo::fontColor);
        }
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
//        Print << U"Track {}"_fmt(getData().trackCount);
//        Print << U"MusicSelect";
        
        TextureAsset(U"select").drawAt(Window::Center());
        
        for (auto i : step(music.size())) {
            music.at(i).draw(Vec2(((int)i - (int)getData().currentMusic) * defaultEntity.w, 0));
        }
    }

}
