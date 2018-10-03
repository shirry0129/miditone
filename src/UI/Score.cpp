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
        
        int hitCount = 0;
        int holdCount = 0;
        
        for(auto i:step(hit.size() + hold.size())){
            if (hit.at(hitCount).judgeTime >= hold.at(holdCount).startTime) {
                score.push_back(&hold.at(holdCount));
                if(++holdCount > hold.size() - 1){
                    holdCount = hold.size() - 1;
                }
            }else{
                score.push_back(&hit.at(hitCount));
                if(++hitCount > hit.size() - 1){
                    hitCount = hit.size() - 1;
                }
            }
        }
    }
    
    void Score::update(double currentTime) {
        for(auto &n:score){
            n->update(currentTime);
        }
    }
    
    void Score::draw() {
        for (auto n:score) {
            n->draw();
        }
    }
    
    
}
