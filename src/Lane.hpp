//
//  Lane.h
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/15.
//

#ifndef Lane_h
#define Lane_h

#include<Siv3D.hpp>

namespace Lane {

    class LaneBG {
    private:
        LaneBG();
        ~LaneBG() = default;
        Array<Line> guide;
        Line underLine;
        Vec2 laneBegin;
        Vec2 interval;
        Vec2 leftEnd, rightEnd;

        static LaneBG* instance;

    public:
        LaneBG(const LaneBG&) = delete;
        LaneBG& operator=(const LaneBG&) = delete;
        LaneBG(LaneBG&&) = delete;
        LaneBG& operator=(LaneBG&&) = delete;

        static LaneBG& getInstance();
        static void create();
        static void destroy();

        void update();
        void draw();
    };

}

#endif /* Lane_h *///
