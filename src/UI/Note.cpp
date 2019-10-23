//
//  Note.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/22.
//

#include "Note.hpp"

using namespace ui;

Note::Note(size_t _laneNum, float _wakeUpTime):
laneNum(_laneNum),
wakeUpTime(_wakeUpTime),
isJudged(false),
isJudging(false){}

void Note::makeJudged() {
    isJudged = true;
}

void Note::makeJudging() {
    isJudging = true;
}
