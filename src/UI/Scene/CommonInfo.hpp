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

namespace ui{
    
    struct PlayData {
        size_t trackCount;
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
