//
//  Result.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Result.hpp"
#include "../../GameInfo.hpp"

namespace ui{

    Result::Result(const InitData& init):IScene(init) {}
    
    void Result::update() {
        if (button.pressed()) {
            if (getData().trackCount < gameinfo::totalTrack) {
                changeScene(SceneName::MUSICSELECT, 2000);
            }else{
                changeScene(SceneName::TOTALRESULT, 2000);
            }
        };
    }
    
    void Result::draw() const {
        ClearPrint();
        Print << U"Track {}"_fmt(getData().trackCount);
        Print << U"Result";
        Print << U"Critical:" << getData().decisionCount.at(getData().trackCount - 1).criticalCount <<
        U"  " << U"Correct:" << getData().decisionCount.at(getData().trackCount - 1).correctCount <<
        U"  " << U"Nice:" << getData().decisionCount.at(getData().trackCount - 1).niceCount <<
        U"  " << U"Miss:" << getData().decisionCount.at(getData().trackCount - 1).missCount;
        Print << U"Combo:" << getData().decisionCount.at(getData().trackCount - 1).combo;
        Print << U"Score:" << getData().resultScore.at(getData().trackCount - 1);
    }

}
