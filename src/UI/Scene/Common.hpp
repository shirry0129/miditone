//
//  CommonInfo.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef CommonInfo_h
#define CommonInfo_h

#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "../../System/Score.hpp"
#include "../../GameInfo.hpp"

namespace ui{
    
    struct IBox{
        Texture design;
        RectF entity;
        IBox(const RectF& _entity):entity(_entity){};
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
    
    struct PlayData {
        size_t trackCount;
        FilePath scoreFile;
        FilePath musicFile;
        double decisionVolume;
        double speed;
        size_t currentDiff;
        Array<DecisionInfo> decisionCount;
        Array<score::Score::Header> resultSongInfo;
        Array<score::Difficulty> resultDiff;
        Array<size_t> resultScore;
    };
    
    enum class SceneName {
        TITLE,
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
