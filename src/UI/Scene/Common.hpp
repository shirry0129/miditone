//
//  CommonInfo.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef CommonInfo_h
#define CommonInfo_h

#include <Siv3D.hpp>
#include "../../System/SystemScore.hpp"
#include "../../GameInfo.hpp"
#include "../../System/MiditoneClient.hpp"

namespace ui{
    
    struct IBox{
        Texture design;
        RectF entity;
        IBox(const RectF& _entity, const Texture& _design):
        entity(_entity),
        design(_design){};
    };
    
    struct DecisionInfo{
        size_t combo;
        size_t criticalCount;
        size_t correctCount;
        size_t niceCount;
        size_t missCount;
        DecisionInfo():
        combo(0),
        criticalCount(0),
        correctCount(0),
        niceCount(0),
        missCount(0){};
    };

    struct PastScore {
        size_t point;
        size_t combo;
        PastScore():
        point(0),
        combo(0){};
    };

    struct HighScore {
        PastScore easy;
        PastScore normal;
        PastScore hard;
    };

    struct ScoreData {
        std::string scorePath;
        score::Header songInfo;
        HighScore highScore;
        FilePath musicPath;
        FilePath jacketPath;
        FilePath exPath;
        FilePath exNote;
    };
    
    struct PlayData {
        size_t trackCount;
        double decisionVolume;
        double speed;
        size_t currentDiff;
        Array<ScoreData>::iterator currentMusic;
        Array<ScoreData> scoreList;
        Array<DecisionInfo> decisionCount;
        Array<std::pair<ScoreData, score::Difficulty>> resultSongInfo;
        Array<size_t> resultScore;
        api_client::MiditoneClient client;
        api_client::response::user_attr user;
        bool isGuest;
        Timer selectTimer;
        bool isSelecting;
    };
    
    enum class SceneName {
        TITLE,
        AUTHENTICATION,
        MUSICSELECT,
        PREFERENCE,
        PLAY,
        RESULT,
        TOTALRESULT,
        GAMEOVER
    };
    
    static auto button = KeyD | KeyF | KeyJ | KeyK;
    
    using MyApp = SceneManager<SceneName, PlayData>;

}

#endif /* CommonInfo_h */
