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
}
