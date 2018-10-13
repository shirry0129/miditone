//
//  Title.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Title_hpp
#define Title_hpp

#include "Common.hpp"

namespace ui{

    class Title : public MyApp::Scene{
    private:
    public:
        Title(const InitData& init);
        void update() override;
        void draw() const override;
    };

}
#endif /* Title_hpp */
