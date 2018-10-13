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
#include "../../System/ScoreManager.hpp"
#include "../../GameInfo.hpp"

namespace ui{
    
    struct IBox{
        String m_content;
        Texture design;
        RectF entity;
        IBox(const String& _content, const RectF& _entity);
    };
    
    struct PlayData {
        size_t trackCount;
        FilePath scoreFile;
        double decisionVolume;
        double speed;
        Array<score::ScoreManager::Header> songInfo;
        size_t currentDiff;
        Array<score::Difficulty> resultDiff;
        Array<size_t> resultScore;
    };
    
    enum class SceneName {
        TITLE,
        MUSICSELECT,
        PREFERENCE,
        PLAY,
        RESULT,
        GAMEOVER
    };
    
    static auto button = KeyD | KeyF | KeyJ | KeyK;
    
    using MyApp = SceneManager<SceneName, PlayData>;

}

#endif /* CommonInfo_h */
