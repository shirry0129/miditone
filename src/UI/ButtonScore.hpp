//
//  Score.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/27.
//

#ifndef Score_hpp
#define Score_hpp

#include <Siv3D.hpp>
#include "ButtonNote.hpp"
#include "../System/SystemScore.hpp"

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
    
    class ButtonScore {
    private:
        Array<Array<ButtonHitNote>> hit;
        Array<Array<ButtonHoldNote>> hold;
        Array<Array<ButtonNote*>> score;
        Array<Bar> bar;
        float wakeUpTime;
        float acceleration;
        
    public:
        ButtonScore();
        ButtonScore(const score::SystemScore& _fromFile, float _speed);
        ~ButtonScore() = default;
        void setFromFile(const score::SystemScore& _fromFile, float _speed);
        float getWakeUpTime() const;
        void deleteJudgedNote(size_t _lane, int spot);
        void adjustJudgingNote(size_t _lane, int spot);
        
        void update(double currentTime);
        void draw() const;
    };
    
}

#endif /* Score_hpp */
