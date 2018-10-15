//
//  MusicSelect.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "MusicSelect.hpp"

namespace ui{
    
    MusicBox::MusicBox(const FilePath& _info, const s3d::RectF &_entity):
    IBox(_entity),
    scoreFile(_info),
    musicInfo(_info.toUTF32()){}
    
    void MusicBox::draw(const s3d::Vec2 &moveWidth) const{
        if (moveWidth.x == 0) {
            entity.movedBy(moveWidth).draw(Palette::Green).drawFrame(4, Palette::Darkgreen);
        }else{
            entity.movedBy(moveWidth).scaled(0.8).draw(Palette::Green).drawFrame(4, Palette::Darkgreen);
        }
        FontAsset(U"BoxFont")(musicInfo.title()).drawAt(entity.center() + moveWidth + Vec2(0, -150));
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
    currentItem(0),
    defaultEntity(Arg::center(Window::Center()), boxSize) {
        getData().trackCount++;
        for (auto file : FileSystem::DirectoryContents(U"../Score/score")) {
            if (FileSystem::Extension(file) == U"txt") {
                music.emplace_back(file, defaultEntity);
            }
        }
    }
    
    void MusicSelect::update() {
        if (gameinfo::backArrow.down()) {
            if (currentItem > 0) {
                currentItem--;
            }
        }
        if (gameinfo::goArrow.down()) {
            if (currentItem < music.size() - 1) {
                currentItem++;
            }
        }
        if (gameinfo::decide.down()) {
            getData().scoreFile = music.at(currentItem).getScoreFile();
            getData().musicFile = U"../Score/music/{}.mp3"_fmt(music.at(currentItem).getMusicInfo().id());
            changeScene(SceneName::PREFERENCE, 2000);
        };
    }
    
    void MusicSelect::draw() const {
        ClearPrint();
        Print << U"Track {}"_fmt(getData().trackCount);
        Print << U"MusicSelect";
        
        for (auto i : step(music.size())) {
            music.at(i).draw(Vec2(((int)i - (int)currentItem) * defaultEntity.w, 0));
        }
    }

}
