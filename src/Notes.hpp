//
//  Notes.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#ifndef Notes_hpp
#define Notes_hpp

#include <Siv3D.hpp>

enum class NotesType {
    Hit,
    Hold
};

class Notes {
private:
    RectF note;
    Array<Effect> decisionEffect;
    Texture looks;
    NotesType notesType;
    int laneNum;
    int startTime;
    int endTime;
    
public:
    Notes(int _laneNum, int _startTime);
    Notes(int _laneNum, int _startTime, int _endTime);
    ~Notes() = default;
    
    void update(double msec);
    void draw();
};

#endif /* Notes_hpp */
