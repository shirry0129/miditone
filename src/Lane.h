//
//  Lane.h
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/15.
//

#ifndef Lane_h
#define Lane_h

#include<Siv3D.hpp>

class Lane {
private:
    Lane();
    ~Lane();
    Array<Line> guide;
    Line underLine;
    Vec2 laneBegin;
    Vec2 interval;
    Vec2 leftEnd, rightEnd;

    static Lane* instance;

public:
    Lane(const Lane&) = delete;
    Lane& operator=(const Lane&) = delete;
    Lane(Lane&&) = delete;
    Lane& operator=(Lane&&) = delete;

    static Lane& getInstance();
    static void create();
    static void destroy();

    void update();
    void draw();
};

#endif /* Lane_h *///
