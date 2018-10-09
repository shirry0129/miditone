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
            System::Exit();
        };
    }
    
    void GameOver::draw() const {
        ClearPrint();
        Print << U"GameOver";
    }

}
