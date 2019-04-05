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

	const Size originalResolution = {1920, 1080};
	
	const Size originalScreenCenter = originalResolution / 2;

    const String title = U"miditone";
    
    const Color backGroundColor = Palette::Yellow;
    
    const Color fadeColor = Palette::White;
    
    const Color fontColor = Palette::Paleturquoise;
    
    const Color easy = Palette::Limegreen;
    const Color normal = Palette::Darkorange;
    const Color hard = Palette::Crimson;
    
    const int32 titleFontSize = 120;
    
    const int32 defalutFontSize = 50;
    
    const int32 totalTrack = 2;
    
    const Array<FileFilter> scoreData = {{U"譜面データ", {U"txt"}}};
    
    const auto backArrow = KeyD;
    const auto goArrow = KeyF;
    const auto decide = KeyJ;
    const auto back = KeyK;
    
    const size_t maxSpeed = 15;
    
    const size_t maxPoint = 1000000;
    
    const size_t fadeTime = 1000;
    
    const SecondsF exFadeTime = 3s;
}

#endif /* GameInfo_h */
