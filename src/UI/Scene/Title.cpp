//
//  Title.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Title.hpp"

namespace ui{

    Title::Title(const InitData& init):IScene(init) {
        getData().trackCount = 0;
        getData().decisionVolume = 8;
        getData().speed = 5;
        getData().currentDiff = 1;
    }
    
    void Title::update() {
        if (button.down()) {
            changeScene(SceneName::MUSICSELECT, 2000);
        };
    }
    
    void Title::draw() const {
        ClearPrint();
        Print << U"Title";
        Print << U"Press any button";
    }

}
