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
    const Array<float> intercept = { 835, 897.5, 960, 1022.5, 1085 };
    const Array<float> slope = { -0.625, -0.3125, 0, 0.3125, 0.625 };
    const int laneEnd = 1000;

    class LaneBG {
    private:
        LaneBG();
        ~LaneBG() = default;
        Array<Line> guide;
        Line decisionLine;
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

#endif /* Lane_h */
