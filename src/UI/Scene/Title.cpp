//
//  Title.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Title.hpp"

namespace ui{

    Title::Title(const InitData& init):IScene(init) {
        getData().trackCount = 0;
        getData().decisionVolume = 8;
        getData().speed = 7.5;
        getData().currentDiff = 1;
        getData().currentMusic = 0;
        getData().resultSongInfo.clear();
        getData().resultScore.clear();
        getData().decisionCount.clear();
        
        if (getData().scoreList.isEmpty()) {
            getData().scoreList = FileSystem::DirectoryContents(U"../Score/score")
            .keep_if([](FilePath t){
                return FileSystem::Extension(t) == U"txt";
            });
        }
    }
    
    void Title::update() {
        if (button.down()) {
            changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
        };
    }
    
    void Title::draw() const {
        ClearPrint();
		TextureAsset(U"title").drawAt(::gameinfo::originalScreenCenter);
        FontAsset(U"infoFont")(U"Press Any Button").drawAt(::gameinfo::originalScreenCenter + Vec2(0, 440), ColorF(gameinfo::fontColor, Periodic::Sine0_1(1.5s)));
    }

}
