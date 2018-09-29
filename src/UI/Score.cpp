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
                    score.emplace_back(n.lane, n.t_beg.sec, _speed);
                    break;
                    
                case score::NoteType::HOLD:
                    score.emplace_back(n.lane, n.t_beg.sec, n.t_end.sec, _speed);
                    break;
            }
        }
    }
    
    void Score::update(double currentTime) {
        for(auto &n:score){
            n.update(currentTime);
        }
    }
    
    void Score::draw() {
        for (auto n:score) {
            n.draw();
        }
    }
    
}
