//
//  Note.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#ifndef Notes_hpp
#define Notes_hpp

#include <Siv3D.hpp>
#include "Lane.hpp"
#include "../System/Score.hpp"

namespace ui{
    
    constexpr int xMax = laneEnd + 500;
    
    class Note {
    protected:
        const LaneBG& inst;
        const size_t laneNum;
        const float wakeUpTime;
        const float acceleration;
        Quad note;
        bool isJudged;
        bool isJudging;
        
    public:
        Note(size_t _laneNum, float _wakeUpTime, float _acceleration);
        ~Note() = default;
        virtual void makeJudged() final;
        virtual void makeJudging() final;
        
        virtual void update(double currentTime) = 0;
        virtual void draw() const = 0;
    };
    
    class HitNote final : public Note {
    private:
        float height;
        
    public:
        const float judgeTime;
        HitNote(size_t _laneNum, float _judgeTime, float _wakeUpTime, float _acceleration);
        
        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };
    
    class HoldNote final : public Note {
    private:
        Quad start;
        Quad end;
    public:
        const float startTime;
        const float endTime;
        
        HoldNote(size_t _laneNum, float _startTime, float _endTime, float _wakeUpTime, float _acceleration);
        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };
}

#endif /* Notes_hpp */
