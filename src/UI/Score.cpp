//
//  Score.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#include "Score.hpp"


namespace ui {
    
    Score::Score(const std::vector<score::Note>& _fromFile, float _speed) {
        setFromFile(_fromFile, _speed);
    }
    
    void Score::setFromFile(const std::vector<score::Note>& _fromFile, float _speed) {
        for(auto n:_fromFile){
            switch (n.type) {
                case score::NoteType::HIT:
                    hit.emplace_back(n.lane, n.t_beg.sec, _speed);
                    break;
                    
                case score::NoteType::HOLD:
                    hold.emplace_back(n.lane, n.t_beg.sec, n.t_end.sec, _speed);
                    break;
            }
        }
        
        auto hitCount = hit.begin();
        auto holdCount = hold.begin();
        
        while (hitCount != hit.end() || holdCount != hold.end()) {
            
            if(hitCount == hit.cend()){
                for (; holdCount != hold.cend(); holdCount++) {
                    score.push_back(&(*holdCount));
                }
            }else if(holdCount == hold.cend()){
                for (; hitCount != hit.cend(); hitCount++) {
                    score.push_back(&(*hitCount));
                }
            }else if(hitCount->judgeTime > holdCount->startTime){
                score.push_back(&(*holdCount));
                ++holdCount;
            }else if(holdCount->startTime > hitCount->judgeTime){
                score.push_back(&(*hitCount));
                ++hitCount;
            }else{
                score.push_back(&(*holdCount));
                score.push_back(&(*hitCount));
                ++holdCount;
                ++hitCount;
            }
            
        }
        
//        for(auto i:step(hit.count() + hold.count())){
//            if(hitCount->judgeTime >= holdCount->startTime){
//                score.push_back(&(*holdCount));
//                if(holdCount != hold.end()){
//                    ++holdCount;
//                }
//            }else{
//                score.push_back(&(*hitCount));
//                if(hitCount != hit.end()){
//                    ++hitCount;
//                }
//            }
//        }
    }
    
    void Score::update(double currentTime) {
        for(auto &n:score){
            n->update(currentTime);
        }
    }
    
    void Score::draw() const{
        for (auto n:score) {
            n->draw();
        }
    }
    
    
}
