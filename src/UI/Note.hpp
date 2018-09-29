//
//  Note.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#ifndef Notes_hpp
#define Notes_hpp

#include <Siv3D.hpp>
#include "../System/ScoreManager.hpp"

namespace ui{
    
    class Note {
    protected:
        Texture looks;
        int laneNum;
        float startTime;
        float endTime;
        float speed;
        const score::NoteType nType;
        Quad note;
        
        void hitLocate(double currentTime);
        void holdLocate(double currentTime);
        
    public:
        Note(int _laneNum, float _startTime, float _speed);
        Note(int _laneNum, float _startTime, float _endTime, float _speed);
        ~Note() = default;
        
        void update(double currentTime);
        void draw();
    };
    
}

#endif /* Notes_hpp */
