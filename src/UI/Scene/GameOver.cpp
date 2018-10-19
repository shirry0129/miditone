//
//  GameOver.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "GameOver.hpp"

namespace ui{

    GameOver::GameOver(const InitData& init):IScene(init){
        changeTime.addEvent(U"Restart", 10s);
        
        changeTime.start();
    }
    
    void GameOver::update() {
        changeTime.update();
        
        if (KeyR.pressed() || changeTime.onTriggered(U"Restart")) {
            changeScene(SceneName::TITLE, gameinfo::fadeTime);
        };
    }
    
    void GameOver::draw() const {
        TextureAsset(U"gameover").drawAt(Window::Center());
    }

}
