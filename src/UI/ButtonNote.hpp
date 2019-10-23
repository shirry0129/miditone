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
#include "Note.hpp"
#include "../System/SystemScore.hpp"

namespace ui{
    
    constexpr int xMax = laneEnd + 500;
    
    class ButtonNote : public Note {
    protected:
        const LaneBG& inst;
        const float acceleration;
        
    public:
        ButtonNote(size_t _laneNum, float _wakeUpTime, float _acceleration);
        ~ButtonNote() = default;
        
        virtual void update(double currentTime) = 0;
        virtual void draw() const = 0;
    };
    
    class ButtonHitNote final : public ButtonNote {
    private:
        float height;
        
    public:
        const float judgeTime;
        ButtonHitNote(size_t _laneNum, float _judgeTime, float _wakeUpTime, float _acceleration);
        
        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };
    
    class ButtonHoldNote final : public ButtonNote {
    private:
        Quad start;
        Quad end;
    public:
        const float startTime;
        const float endTime;
        
        ButtonHoldNote(size_t _laneNum, float _startTime, float _endTime, float _wakeUpTime, float _acceleration);
        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };
}

#endif /* Notes_hpp */
