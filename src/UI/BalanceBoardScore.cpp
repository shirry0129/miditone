//
//  BalanceBoardScore.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/22.
//

#include "BalanceBoardScore.hpp"
#include "Lane.hpp"
#include "../GameInfo.hpp"

using namespace ui;

BalanceBoardScore::BalanceBoardScore():
hit(numOfLane),
hold(numOfLane),
score(numOfLane){}

BalanceBoardScore::BalanceBoardScore(const score::SystemScore &_fromFile, float _speed, const Array<double>& laneAngle):
hit(numOfLane),
hold(numOfLane),
score(numOfLane){ 
    setFromFile(_fromFile, _speed, laneAngle);
}

void BalanceBoardScore::setFromFile(const score::SystemScore &_fromFile, float _speed, const Array<double>& laneAngle) { 
    double speed = (lineRadius - 100) * _speed + 100;
    wakeUpTime = (lineRadius - 100) / speed;
    
    for (const auto& n : _fromFile.getNotes()) {
        switch (n.type) {
            case score::NoteType::HIT:
                hit.at(n.lane).emplace_back(n.lane, n.t_beg.sec, wakeUpTime, laneAngle.at(n.lane), speed);
                break;
                
            case score::NoteType::HOLD:
                hold.at(n.lane).emplace_back(n.lane, n.t_beg.sec, n.t_end.sec, wakeUpTime, laneAngle.at(n.lane), speed);
        }
    }
    
    for (const auto &i : step(numOfLane)) {
        auto hitCount = hit.at(i).begin();
        auto holdCount = hold.at(i).begin();
        
        while (hitCount != hit.at(i).cend() || holdCount != hold.at(i).cend()) {
            if (hitCount == hit.at(i).cend()) {
                for (; holdCount != hold.at(i).cend(); holdCount++) {
                    score.at(i).push_back(&(*holdCount));
                }
            } else if (holdCount == hold.at(i).cend()) {
                for (; hitCount != hit.at(i).cend(); hitCount++) {
                    score.at(i).push_back(&(*hitCount));
                }
            } else if (hitCount->judgeTime > holdCount->startTime) {
                score.at(i).push_back(&(*holdCount));
                ++holdCount;
            } else if (holdCount->startTime > hitCount->judgeTime) {
                score.at(i).push_back(&(*hitCount));
                ++hitCount;
            } else {
                score.at(i).push_back(&(*holdCount));
                score.at(i).push_back(&(*hitCount));
                ++holdCount;
                ++hitCount;
            }
        }
    }
}

float BalanceBoardScore::getWakeUpTime() { 
    return wakeUpTime;
}

void BalanceBoardScore::deleteJudgedNote(size_t _lane, int spot) { 
    score.at(_lane).at(spot)->makeJudged();
}

void BalanceBoardScore::adjustJudgingNote(size_t _lane, int spot) { 
    score.at(_lane).at(spot)->makeJudging();
}

void BalanceBoardScore::update(double currentTime) { 
    for (auto &l : score) {
        for (auto &n : l) {
            n->update(currentTime);
        }
    };
}

void BalanceBoardScore::draw() const {
    for (const auto &l : score) {
        for (const auto &n : l) {
            n->draw();
        }
    };
}
