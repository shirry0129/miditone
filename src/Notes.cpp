//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Notes.hpp"


Notes::Notes(int _laneNum, int _startTime):
	notesType(NotesType::Hit),
	laneNum(_laneNum),
	startTime(_startTime),
	endTime(NULL){}

Notes::Notes(int _laneNum, int _startTime, int _endTime):
	notesType(NotesType::Hold),
	laneNum(_laneNum),
	startTime(_startTime),
	endTime(_endTime){}

void Notes::update(double msec) {
	note.set(835, -5, 62.5, 10);
}

void Notes::draw() { 
    note.draw();
}
