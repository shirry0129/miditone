//
//  GameOver.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef GameOver_hpp
#define GameOver_hpp

#include "CommonInfo.hpp"

namespace ui{

    class GameOver : public MyApp::Scene {
    private:
    public:
        GameOver(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* GameOver_hpp */
