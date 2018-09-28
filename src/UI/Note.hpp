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
        const score::NoteType nType;
        Quad note;
        void hitLocate(double currentTime, float speed);
        void holdLocate(double currentTime, float speed);
        
    public:
        Note(int _laneNum, float _startTime);
        Note(int _laneNum, float _startTime, float _endTime);
        ~Note() = default;
        
        void update(double currentTime, float speed);
        void draw();
    };
    
}

#endif /* Notes_hpp */
