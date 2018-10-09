//
//  Preference.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Preference_hpp
#define Preference_hpp

#include "CommonInfo.hpp"

namespace ui{

    class Preference : public MyApp::Scene {
    private:
    public:
        Preference(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Preference_hpp */
