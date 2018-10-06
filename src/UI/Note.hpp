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
#include "../System/ScoreManager.hpp"

namespace ui{
    
    class Note {
    protected:
        Texture looks;
        size_t laneNum;
        float speed;
        Quad note;
        LaneBG& inst;
        
    public:
        Note(size_t _laneNum, float _speed);
        ~Note() = default;
        
        virtual void update(double currentTime) = 0;
        virtual void draw() const final;
    };
    
    class HitNote final : public Note {
    private:
        float height;
        
    public:
        const float judgeTime;
        HitNote(size_t _laneNum, float _judgeTime, float _speed);
        
        virtual void update(double currentTime) override;
    };
    
    class HoldNote final : public Note {
    public:
        const float startTime;
        const float endTime;
        
        HoldNote(size_t _laneNum, float _startTime, float _endTime, float _speed);
        virtual void update(double currentTime) override;
    };
}

#endif /* Notes_hpp */
