//
//  Score.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#include "Score.hpp"


namespace ui {
    
    Score::Score():
    hit(numObLane),
    hold(numObLane),
    score(numObLane){}
    
    Score::Score(const std::vector<score::Note>& _fromFile, float _speed):
    hit(numObLane),
    hold(numObLane),
    score(numObLane){
        setFromFile(_fromFile, _speed);
    }
    
    void Score::setFromFile(const std::vector<score::Note>& _fromFile, float _speed) {
        for(auto n:_fromFile){
            switch (n.type) {
                case score::NoteType::HIT:
                    hit.at(n.lane).emplace_back(n.lane, n.t_beg.sec, _speed);
                    break;
                    
                case score::NoteType::HOLD:
                    hold.at(n.lane).emplace_back(n.lane, n.t_beg.sec, n.t_end.sec, _speed);
                    break;
            }
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
        for(auto &l:score){
            for(auto &n:l){
                n->update(currentTime);
            }
        }
    }
    
    void Score::draw() const{
        for (auto l:score) {
            for(auto n:l){
                n->draw();
            }
        }
    }
    
    void Score::deleteJudgedNote(size_t _lane, int spot) {
        score.at(_lane).at(spot)->makeJudged();
    }
    
}
