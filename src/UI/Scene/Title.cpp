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
        getData().resultSongInfo.clear();
        getData().resultScore.clear();
        getData().decisionCount.clear();
        getData().isGuest = true;

        if (getData().scoreList.isEmpty()) {
            for (const auto& songDir : FileSystem::DirectoryContents(U"../Score").filter(FileSystem::IsDirectory)) {
                for (const auto& ini : FileSystem::DirectoryContents(songDir).filter(
                    [](FilePath path){return FileSystem::FileName(path) == U"score.ini";}
                )) {
                    const INIData music(ini);
                    ScoreData data;
                    
                    data.scorePath = (songDir + music[U".score"].removed(U'"')).narrow();
                    if (data.songInfo.read(data.scorePath).isError())
                        continue;
                    data.musicPath = songDir + music[U".music"].removed(U'"');
                    data.jacketPath = songDir + music[U".jacket"].removed(U'"');
                    data.exPath = songDir + music[U".musicEx"].removed(U'"');
                    data.exNote = songDir + music[U".exNote"].removed(U'"');
                    
                    getData().scoreList << data;
                }
            }
        } else {
            for (auto& score: getData().scoreList) {
                score.highScore.clear();
            }
        }
        
        getData().currentMusic = getData().scoreList.begin();
    }
    
    void Title::update() {
        if (gameinfo::button.down()) {
            changeScene(SceneName::AUTHENTICATION, gameinfo::fadeTime);
        };
    }
    
    void Title::draw() const {
		TextureAsset(U"title").drawAt(::gameinfo::originalScreenCenter);
        FontAsset(U"50")(U"Press Any Button").drawAt(::gameinfo::originalScreenCenter + Vec2(0, 440), ColorF(gameinfo::defaultFontColor, Periodic::Sine0_1(1.5s)));
#ifdef MIDITONE_WIIBALANCEBOARD
        FontAsset(U"15")(U"Battery {:3} %"_fmt(gameinfo::balanceBoard.battery_level() * 100))
            .draw(Arg::bottomLeft = Vec2(10, gameinfo::originalResolution.y - 20));
#endif
    }

}
