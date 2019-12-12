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
score(numOfLane){}

BalanceBoardScore::BalanceBoardScore(const score::SystemScore &_fromFile, float _speed, const Array<double>& laneAngle):
score(numOfLane){ 
    setFromFile(_fromFile, _speed, laneAngle);
}

void BalanceBoardScore::setFromFile(const score::SystemScore &_fromFile, float _speed, const Array<double>& laneAngle) { 
    double speed = (lineRadius - 100) * _speed + 100;
    wakeUpTime = (lineRadius - 100) / speed;
    
    BalanceBoardNote *prevNote = nullptr;
    double prevTime = 0;
    
    
    for (const auto& n : _fromFile.getNotes()) {
        bool isConcurrent = false;
        
        if (prevTime == n.t_beg.sec && prevNote != nullptr) {
            isConcurrent = true;
            prevNote->setIsConcurrent(true);
        }
        
        switch (n.type) {
            case score::NoteType::HIT:
                score.at(n.lane).emplace_back(
                    std::make_unique<BalanceBoardHitNote>(
                        n.lane, n.t_beg.sec, wakeUpTime, laneAngle.at(n.lane), speed, isConcurrent
                    )
                );
                break;
                
            case score::NoteType::HOLD:
                score.at(n.lane).emplace_back(
                    std::make_unique<BalanceBoardHoldNote>(
                        n.lane, n.t_beg.sec, n.t_end.sec, wakeUpTime, laneAngle.at(n.lane), speed, isConcurrent
                        )
                    );
                break;
        }
        
        prevTime = n.t_beg.sec;
        prevNote = score.at(n.lane).back().get();
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
