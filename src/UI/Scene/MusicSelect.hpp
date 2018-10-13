//
//  MusicSelect.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef MusicSelect_hpp
#define MusicSelect_hpp

#include "Common.hpp"

namespace ui{

    class MusicSelect : public MyApp::Scene{
    private:
    public:
        MusicSelect(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* MusicSelect_hpp */
