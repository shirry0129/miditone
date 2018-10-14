//
//  TotalResult.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/13.
//

#ifndef TotalResult_hpp
#define TotalResult_hpp

#include "Common.hpp"

namespace ui {
    
    class TotalResult : public MyApp::Scene {
    private:
    public:
        TotalResult(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* TotalResult_hpp */
