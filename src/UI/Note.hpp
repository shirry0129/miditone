//
//  Note.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#ifndef Notes_hpp
#define Notes_hpp

#include <Siv3D.hpp>

namespace ui{
    
    class Note {
    protected:
        Array<Effect> decisionEffect;
        Texture looks;
        int laneNum;
        
    public:
        Note(int _laneNum);
        ~Note() = default;
        
        virtual void update(double currentTime, float speed) = 0;
        virtual void draw() = 0;
    };
    
    class HitNote final: public Note {
    private:
        float startTime;
        RectF note;
    public:
        HitNote(int _laneNum, float _startTime);
        virtual void update(double currentTime, float speed) override;
        virtual void draw() override;
    };
    
    class HoldNote final: public Note {
    private:
        float startTime;
        float endTime;
        Quad note;
    public:
        HoldNote(int _laneNum, float _startTime, float _endTime);
        virtual void update(double currentTime, float speed) override;
        virtual void draw() override;
    };
    
}

#endif /* Notes_hpp */
