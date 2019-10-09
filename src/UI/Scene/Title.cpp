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
        getData().resultSongInfo.clear();
        getData().resultScore.clear();
        getData().decisionCount.clear();
        
        if (getData().scoreList.isEmpty()) {
            for (const auto& songDir : FileSystem::DirectoryContents(U"../Score").filter(FileSystem::IsDirectory)) {
                for (const auto& ini : FileSystem::DirectoryContents(songDir).filter(
                    [](FilePath path){return FileSystem::FileName(path) == U"score.ini";}
                )) {
                    const INIData music(ini);
                    ScoreData data;
                    
                    data.scorePath = (songDir + music[U".score"].removed(U'"')).toUTF32();
                    data.songInfo.read(data.scorePath);
                    data.musicPath = songDir + music[U".music"].removed(U'"');
                    data.jacketPath = songDir + music[U".jacket"].removed(U'"');
                    data.exPath = songDir + music[U".musicEx"].removed(U'"');
                    data.exNote = songDir + music[U".exNote"].removed(U'"');
                    
                    getData().scoreList << data;
                }
            }
        }
        
        getData().currentMusic = getData().scoreList.begin();
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
