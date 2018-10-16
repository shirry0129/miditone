//
//  Score.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#ifndef Score_hpp
#define Score_hpp

#include <Siv3D.hpp>
#include "Note.hpp"
#include "../System/Score.hpp"

namespace ui {
    
    class Bar{
    private:
        const LaneBG& inst;
        const float wakeUpTime;
        const float acceleration;
        const float timeOnLine;
        Line bar;
    public:
        Bar(float _timeOnLine, float _wakeUpTime, float _acceleration);
        void update(double currentTime);
        void draw() const;
    };
    
    class Score {
    private:
        Array<Array<HitNote>> hit;
        Array<Array<HoldNote>> hold;
        Array<Array<Note*>> score;
        Array<Bar> bar;
        float wakeUpTime;
        float acceleration;
        
    public:
        Score();
        Score(const score::Score& _fromFile, float _speed);
        ~Score() = default;
        void setFromFile(const score::Score& _fromFile, float _speed);
        float getWakeUpTime() const;
        void deleteJudgedNote(size_t _lane, int spot);
        
        void update(double currentTime);
        void draw() const;
    };
    
}

#endif /* Score_hpp */
