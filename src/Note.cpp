//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"
#include "Lane.hpp"


/**
 constructor(tap)
 @param _laneNum レーン番号
 @param _startTime 判定時間
 */
Note::Note(int _laneNum, float _startTime):
	notesType(NotesType::Hit),
	laneNum(_laneNum),
	startTime(_startTime),
	endTime(NULL){}

/**
 Constructor(hold)

 @param _laneNum レーン番号
 @param _startTime 頭の時間
 @param _endTime 終わりの時間
 */
Note::Note(int _laneNum, float _startTime, float _endTime):
	notesType(NotesType::Hold),
	laneNum(_laneNum),
	startTime(_startTime),
	endTime(_endTime){}

/**
 次に描画する位置を決定する

 @param currentTime 現在の再生時間
 @param speed ノーツスピード
 */
void Note::update(double currentTime, double speed) {
	float nextY = Lane::decisionLine - ((startTime - currentTime) * Lane::decisionLine * speed);
	float nextX = Lane::slope[laneNum - 1] * nextY + Lane::intercept[laneNum -1];
	float nextWidth = Lane::slope[laneNum] * nextY + Lane::intercept[laneNum] - nextX;
	
	note.set(nextX, nextY, nextWidth, 10);
}

/**
 ノーツの描画
 */
void Note::draw() {
	if(note.x > 0 || note.x < Window::Height()){
    	note.draw();
	}
}
