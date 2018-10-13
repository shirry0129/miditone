//
//  Result.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Result_hpp
#define Result_hpp

#include "Common.hpp"

namespace ui{
    
    class Result : public MyApp::Scene{
    private:
    public:
        Result(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Result_hpp */
