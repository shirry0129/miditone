//
//  TotalResult.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/13.
//

#include "TotalResult.hpp"

using namespace ui;

TotalResult::TotalResult(const InitData &init):IScene(init) {}

void TotalResult::update() {
    if (button.down()) {
        changeScene(SceneName::GAMEOVER, 2000);
    };
}

void TotalResult::draw() const {
    ClearPrint();
    Print << U"TotalResult";
    for (auto r : step(3)) {
        Print << U"Track {}:"_fmt(r + 1) << Unicode::Widen(getData().resultSongInfo.at(r).title);
        Print << U"Critical:" << getData().decisionCount.at(r).criticalCount <<
        U"  " << U"Correct:" << getData().decisionCount.at(r).correctCount <<
        U"  " << U"Nice:" << getData().decisionCount.at(r).niceCount <<
        U"  " << U"Miss:" << getData().decisionCount.at(r).missCount;
        Print << U"Combo:" << getData().decisionCount.at(r).combo;
        Print << U"Score:" << getData().resultScore.at(r) << U"\n";
    }
}
