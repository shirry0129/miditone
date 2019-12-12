//
//  GameInfo.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef GameInfo_h
#define GameInfo_h

#include <Siv3D.hpp>

#ifdef MIDITONE_WIIBALANCEBOARD
#include "System/balance_board/BalanceBoardController.hpp"
#endif

struct gameinfo {

	static const Size originalResolution;
	
	static const Size originalScreenCenter;

    static const String title;
    
    static const Color backGroundColor;
    
    static const Color fadeColor;
    
    static const Color defaultFontColor;
    static const Color infoFontColor;
    
    static const Color easy;
    static const Color normal;
    static const Color hard;
    
    static const Color beamColor;
    
    static const int32 titleFontSize;
    
    static const int32 defalutFontSize;
    
    static int32 totalTrack;
    
    static const Key prev;
    static const Key next;
    static const Key decide;
    static const Key back;
    
    static const size_t maxSpeed;
    
    static const size_t maxPoint;
    
    static const int32 fadeTime;
    
    static const SecondsF exFadeTime;
    
    static const float scale;

#ifdef MIDITONE_WIIBALANCEBOARD
    static AsyncBalanceBoardController balanceBoard;
#endif
};

#endif /* GameInfo_h */
