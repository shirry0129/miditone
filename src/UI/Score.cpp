//
//  Score.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#include "Score.hpp"


namespace ui {
    
    Score::Score(std::vector<score::Note>& _fromFile, float _speed) {
        speed = _speed;
        
        for(auto n: _fromFile){
            switch (n.type) {
                case score::NoteType::HIT:
                    score.emplace_back(n.lane, n.t_beg.sec);
                    break;
                    
                case score::NoteType::HOLD:
                    score.emplace_back(n.lane, n.t_beg.sec, n.t_end.sec);
                    break;
            }
        }
    }
    
    void Score::setFromFile(std::vector<score::Note>& _fromFile, float _speed) {
        speed = _speed;
        
        for(auto n:_fromFile){
            switch (n.type) {
                case score::NoteType::HIT:
                    score.emplace_back(n.lane, n.t_beg.sec);
                    break;
                    
                case score::NoteType::HOLD:
                    score.emplace_back(n.lane, n.t_beg.sec, n.t_end.sec);
                    break;
            }
        }
    }
    
    void Score::update(double currentTime) {
        for(auto n:score){
            n.update(currentTime, speed);
        }
    }
    
    void Score::draw() {
        for (auto n:score) {
            n.draw();
        }
    }
    
}
