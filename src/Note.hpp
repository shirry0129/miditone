//
//  Note.hpp
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

class Note {
private:
    RectF note;
    Array<Effect> decisionEffect;
    Texture looks;
    NotesType notesType;
    int laneNum;
    float startTime;
    float endTime;
    
public:
    Note(int _laneNum, float _startTime);
    Note(int _laneNum, float _startTime, float _endTime);
    ~Note() = default;
    
    void update(double currentTime, double speed);
    void draw();
};

#endif /* Notes_hpp */
