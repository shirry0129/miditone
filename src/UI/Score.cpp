//
//  Score.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#include "Score.hpp"
#include "../GameInfo.hpp"

namespace ui {
    
    Bar::Bar(float _timeOnLine, float _wakeUpTime, float _acceleration):
    wakeUpTime(_wakeUpTime),
    acceleration(_acceleration),
    timeOnLine(_timeOnLine),
    inst(LaneBG::getInstance()){}
    
    void Bar::update(double currentTime) {
        if(wakeUpTime - (timeOnLine - currentTime) < 0){
            bar.set(0, 0, 0, 0);
            return;
        }
        
        float nextY = 0.5 * acceleration * pow(wakeUpTime - (timeOnLine - currentTime), 2) - 500;
        
        Vec2 l(inst.getFactor(0).slope * (nextY) + inst.getFactor(0).intercept + 1, nextY);
        Vec2 r(inst.getFactor(4).slope * (nextY) + inst.getFactor(4).intercept - 1, nextY);
        
        bar.set(l, r);
    }
    
    void Bar::draw() const {
        if (bar.begin.y > 0 && bar.begin.y < ::gameinfo::originalResolution.y) {
            bar.draw(LineStyle::RoundCap, 2, Color(Palette::White, 200));
        }
    }

    
    
    
    Score::Score():
    hit(numObLane),
    hold(numObLane),
    score(numObLane){}
    
    Score::Score(const score::Score& _fromFile, float _speed):
    hit(numObLane),
    hold(numObLane),
    score(numObLane){
        setFromFile(_fromFile, _speed);
    }
    
    void Score::setFromFile(const score::Score& _fromFile, float _speed) {
        wakeUpTime = 2 * xMax / (_speed * xMax);
        acceleration = (_speed * xMax * _speed * xMax) / (2 * xMax);
        
        for(auto n:_fromFile.getNotes()){
            switch (n.type) {
                case score::NoteType::HIT:
                    hit.at(n.lane).emplace_back(n.lane, n.t_beg.sec, wakeUpTime, acceleration);
                    break;
                    
                case score::NoteType::HOLD:
                    hold.at(n.lane).emplace_back(n.lane, n.t_beg.sec, n.t_end.sec, wakeUpTime, acceleration);
                    break;
            }
        }
        
        for (auto b : _fromFile.getBar()) {
            bar.emplace_back(b.time.sec, wakeUpTime, acceleration);
        }
        
        for(auto i:step(numObLane)){
            auto hitCount = hit.at(i).begin();
            auto holdCount = hold.at(i).begin();
            
            while (hitCount != hit.at(i).end() || holdCount != hold.at(i).end()) {
                
                if(hitCount == hit.at(i).cend()){
                    for (; holdCount != hold.at(i).cend(); holdCount++) {
                        score.at(i).push_back(&(*holdCount));
                    }
                }else if(holdCount == hold.at(i).cend()){
                    for (; hitCount != hit.at(i).cend(); hitCount++) {
                        score.at(i).push_back(&(*hitCount));
                    }
                }else if(hitCount->judgeTime > holdCount->startTime){
                    score.at(i).push_back(&(*holdCount));
                    ++holdCount;
                }else if(holdCount->startTime > hitCount->judgeTime){
                    score.at(i).push_back(&(*hitCount));
                    ++hitCount;
                }else{
                    score.at(i).push_back(&(*holdCount));
                    score.at(i).push_back(&(*hitCount));
                    ++holdCount;
                    ++hitCount;
                }
                
            }
        }
    }
    
    void Score::update(double currentTime) {
        for (auto &b : bar) {
            b.update(currentTime);
        }
        
        for(auto &l:score){
            for(auto &n:l){
                n->update(currentTime);
            }
        }
    }
    
    void Score::draw() const{
        for (auto b : bar) {
            b.draw();
        }
        
        for (auto l:score) {
            for(auto n:l){
                n->draw();
            }
        }
    }
    
    void Score::deleteJudgedNote(size_t _lane, int spot) {
        score.at(_lane).at(spot)->makeJudged();
    }
    
    float Score::getWakeUpTime() const {
        return wakeUpTime;
    }
    
    void Score::adjustJudgingNote(size_t _lane, int spot) {
        score.at(_lane).at(spot)->makeJudging();
    }
    
    
    
}
