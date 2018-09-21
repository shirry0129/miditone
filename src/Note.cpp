//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"
#include "Lane.hpp"


/**
 Constructor

 @param _laneNum レーン番号
 */
Note::Note(int _laneNum):laneNum(_laneNum) {}


/**
 Constructor

 @param _laneNum レーン番号
 @param _startTime 判定時間
 */
HitNote::HitNote(int _laneNum, float _startTime):
	Note(_laneNum),
	startTime(_startTime){}

/**
 描画情報の更新

 @param currentTime 現在の再生位置
 @param speed ノーツスピード
 */
void HitNote::update(double currentTime, float speed) {
	float nextY = Lane::laneEnd - ((startTime - currentTime) * Lane::laneEnd * speed);
	float nextX = Lane::slope[laneNum - 1] * nextY + Lane::intercept[laneNum - 1];
	float nextWidth = Lane::slope[laneNum] * nextY + Lane::intercept[laneNum] - nextX;
	
	note.set(nextX + 1, nextY, nextWidth - 2, 10);
}

/**
 ノーツの描画
 */
void HitNote::draw() {
	if(note.y > 0 && note.y < Window::Height()){
		note.draw();
	}
}


/**
 Constructor

 @param _laneNum レーン番号
 @param _startTime 頭の時間
 @param _endTime 終わりの時間
 */
HoldNote::HoldNote(int _laneNum, float _startTime, float _endTime):
	Note(_laneNum),
	startTime(_startTime),
	endTime(_endTime){}

/**
 描画情報の更新

 @param currentTime 現在の再生時間
 @param speed ノーツスピード
 */
void HoldNote::update(double currentTime, float speed) {
	float bottomY = Lane::laneEnd - ((startTime - currentTime) * Lane::laneEnd * speed);
	float topY = Lane::laneEnd - ((endTime - currentTime) * Lane::laneEnd * speed);
	
	if(topY < 0) {
		topY = 0;
	}
	
	Vec2 tl(Lane::slope[laneNum - 1] * topY + Lane::intercept[laneNum - 1] + 1, topY);
	Vec2 tr(Lane::slope[laneNum] * topY + Lane::intercept[laneNum] - 1, topY);
	Vec2 bl(Lane::slope[laneNum - 1] * bottomY + Lane::intercept[laneNum - 1] + 1, bottomY);
	Vec2 br(Lane::slope[laneNum] * bottomY + Lane::intercept[laneNum] - 1, bottomY);
	
	note.set(tl, tr, br, bl);
}

/**
 ノーツ描画
 */
void HoldNote::draw() {
	if(note.p3.y > 0 && note.p0.y < Window::Height()){
		note.draw();
	}
}
