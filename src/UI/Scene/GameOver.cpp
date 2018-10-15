//
//  GameOver.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "GameOver.hpp"

namespace ui{

    GameOver::GameOver(const InitData& init):IScene(init) {}
    
    void GameOver::update() {
        if (button.pressed()) {
            changeScene(SceneName::TITLE, 2000);
        };
    }
    
    void GameOver::draw() const {
        ClearPrint();
        Print << U"GameOver";
    }

}
