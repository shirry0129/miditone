//
//  Preference.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Preference.hpp"

namespace ui{

    Preference::Preference(const InitData &init):IScene(init) {}
    
    void Preference::update() {
        if (button.pressed()) {
            changeScene(SceneName::PLAY, 2000);
        };
    }
    
    void Preference::draw() const {
        ClearPrint();
        Print << U"Track {}"_fmt(getData().trackCount);
        Print << U"Preference";
    }

}
