//
//  MusicSelect.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "MusicSelect.hpp"

namespace ui{

    MusicSelect::MusicSelect(const InitData& init):IScene(init) {
        getData().trackCount++;
    }
    
    void MusicSelect::update() {
        if (button.down()) {
            changeScene(SceneName::PREFERENCE, 2000);
        };
    }
    
    void MusicSelect::draw() const {
        ClearPrint();
        Print << U"Track {}"_fmt(getData().trackCount);
        Print << U"MusicSelect";
    }

}
