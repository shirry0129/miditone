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
                changeScene(SceneName::GAMEOVER, 2000);
            }
        };
    }
    
    void Result::draw() const {
        ClearPrint();
        Print << U"Track {}"_fmt(getData().trackCount);
        Print << U"Result";
    }

}
