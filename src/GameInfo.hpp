//
//  GameInfo.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef GameInfo_h
#define GameInfo_h

#include <Siv3D.hpp>

namespace gameinfo {
    const String title = U"miditone";
    
    const Color backGroundColor = Palette::Yellow;
    
    const Color fadeColor = Palette::White;
    
    const Color fontColor = Palette::Paleturquoise;
    
    const Color easy = Palette::Limegreen;
    const Color normal = Palette::Darkorange;
    const Color hard = Palette::Crimson;
    
    const int32 titleFontSize = 120;
    
    const int32 defalutFontSize = 50;
    
    const int32 totalTrack = 3;
    
    const Array<FileFilter> scoreData = {{U"譜面データ", {U"txt"}}};
    
    const auto backArrow = KeyD;
    const auto goArrow = KeyF;
    const auto decide = KeyJ;
    const auto back = KeyK;
    
    const size_t maxSpeed = 15;
    
    const size_t maxPoint = 1000000;
    
    const size_t fadeTime = 1000;
}

#endif /* GameInfo_h */
