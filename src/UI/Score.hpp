//
//  Score.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#ifndef Score_hpp
#define Score_hpp

#include <Siv3D.hpp>
#include <vector>
#include "Note.hpp"
#include "../System/Score.hpp"

namespace ui {
    
    class Score {
    private:
        Array<Note> score;
        float speed;
        
    public:
        Score(){};
        ~Score() = default;
        Score(std::vector<score::Note>& _fromFile, float _speed);
        void setFromFile(std::vector<score::Note>& _fromFile, float _speed);
        
        void update(double currentTime);
        void draw();
    };
    
}

#endif /* Score_hpp */
