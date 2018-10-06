//
//  Lane.h
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/15.
//

#ifndef Lane_h
#define Lane_h

#include<Siv3D.hpp>

namespace ui {
    constexpr int laneEnd = 1000;
    
    struct factor {
        float slope;
        float intercept;
    };

    class LaneBG {
    private:
        LaneBG();
        ~LaneBG() = default;
        Array<Line> guide;
        Line decisionLine;
        Vec2 laneBegin;
        Vec2 interval;
        Vec2 leftEnd, rightEnd;
        bool isCreated;
        Array<float> slope;
        Array<float> intercept;

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
        void draw() const;
        factor getFactor(size_t guide) const;
    };
}

#endif /* Lane_h */
