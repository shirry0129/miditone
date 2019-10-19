//
//  GameOver.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "GameOver.hpp"

namespace ui{

    GameOver::GameOver(const InitData& init):
    IScene(init),
    changeTime(10){
        changeTime.start();
    }
    
    void GameOver::update() {        
        if (KeyR.down() || changeTime.reachedZero()) {
            changeScene(SceneName::TITLE, gameinfo::fadeTime);
        }
    }
    
    void GameOver::draw() const {
        TextureAsset(U"gameover").drawAt(::gameinfo::originalScreenCenter);
    }

}
