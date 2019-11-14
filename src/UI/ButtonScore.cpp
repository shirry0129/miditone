//
//  Score.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#include "ButtonScore.hpp"
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

    
    
    
    ButtonScore::ButtonScore():
//    hit(numOfLane),
//    hold(numOfLane),
    score(numOfLane){}
    
    ButtonScore::ButtonScore(const score::SystemScore& _fromFile, float _speed):
//    hit(numOfLane),
//    hold(numOfLane),
    score(numOfLane){
        setFromFile(_fromFile, _speed);
    }
    
    void ButtonScore::setFromFile(const score::SystemScore& _fromFile, float _speed) {
        wakeUpTime = 2 * xMax / (_speed * xMax);
        acceleration = (_speed * xMax * _speed * xMax) / (2 * xMax);
        
        for(auto n:_fromFile.getNotes()){
            switch (n.type) {
                case score::NoteType::HIT:
                    score.at(n.lane).emplace_back(
                        std::make_unique<ButtonHitNote>(
                            n.lane, n.t_beg.sec, wakeUpTime, acceleration
                        )
                    );
                    break;
                    
                case score::NoteType::HOLD:
                    score.at(n.lane).emplace_back(
                        std::make_unique<ButtonHoldNote>(
                            n.lane, n.t_beg.sec, n.t_end.sec, wakeUpTime, acceleration
                        )
                    );
                    break;
            }
        }
        
        for (auto b : _fromFile.getBar()) {
            bar.emplace_back(b.time.sec, wakeUpTime, acceleration);
        }
    }
    
    void ButtonScore::update(double currentTime) {
        for (auto &b : bar) {
            b.update(currentTime);
        }
        
        for(auto &l:score){
            for(auto &n:l){
                n->update(currentTime);
            }
        }
    }
    
    void ButtonScore::draw() const{
        for (const auto &b : bar) {
            b.draw();
        }
        
        for (const auto& l:score) {
            for(const auto &n:l){
                n->draw();
            }
        }
    }
    
    void ButtonScore::deleteJudgedNote(size_t _lane, int spot) {
        score.at(_lane).at(spot)->makeJudged();
    }
    
    float ButtonScore::getWakeUpTime() const {
        return wakeUpTime;
    }
    
    void ButtonScore::adjustJudgingNote(size_t _lane, int spot) {
        score.at(_lane).at(spot)->makeJudging();
    }
    
    
    
}
