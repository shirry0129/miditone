//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"

namespace ui {
	
	Note::Note(size_t _laneNum, float _speed):
//	looks(U"example/windmill.png"),
	laneNum(_laneNum),
	wakeUpTime(2 * xMax / (_speed * xMax)),
	acceleration((_speed * xMax * _speed * xMax) / (2 * xMax)),
	isJudged(false),
	inst(ui::LaneBG::getInstance()){}
	
	void Note::draw() const{
		if (!isJudged) {
			if(note.p3.y > 0 && note.p0.y < Window::Height()){
				note.draw(Color(U"#7fffd4"));
				//note(looks).draw();
			}
		}
	}
	
	void Note::makeJudged() {
		isJudged = true;
	}
	
	
	
	
	
	HitNote::HitNote(size_t _laneNum, float _judgeTime, float _speed):
		Note(_laneNum, _speed),
		judgeTime(_judgeTime){}
	
	void HitNote::update(double currentTime) {
		
		if(wakeUpTime - (judgeTime - currentTime) < 0){
			note.set(0, 0, 0, 0, 0, 0, 0, 0);
			return;
		}
		
		float nextY = 0.5 * acceleration * pow(wakeUpTime - (judgeTime - currentTime), 2) - 500;
		float height = 60 * (1. / 1250. * nextY + 0.2);
		
		Vec2 tl(inst.getFactor(laneNum).slope * (nextY - height / 2) + inst.getFactor(laneNum).intercept + 1, nextY - height / 2);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * (nextY - height / 2) + inst.getFactor(laneNum + 1).intercept - 1, nextY - height / 2);
		Vec2 bl(inst.getFactor(laneNum).slope * (nextY + height / 2) + inst.getFactor(laneNum).intercept + 1, nextY + height / 2);
		Vec2 br(inst.getFactor(laneNum + 1).slope * (nextY + height / 2) + inst.getFactor(laneNum + 1).intercept - 1, nextY + height / 2);
		
		note.set(tl, tr, br, bl);
	}
	
	
	
	
	HoldNote::HoldNote(size_t _laneNum, float _startTime, float _endTime, float _speed):
		Note(_laneNum, _speed),
		startTime(_startTime),
		endTime(_endTime){}
	
	void HoldNote::update(double currentTime) {
		
		if (wakeUpTime - (startTime - currentTime) < 0) {
			note.set(0, 0, 0, 0, 0, 0, 0, 0);
			return;
		}
		
		float bottomY = 0.5 * acceleration * pow(wakeUpTime - (startTime - currentTime), 2) - 500;
		
		float topY = 0.5 * acceleration * pow(wakeUpTime - (endTime - currentTime), 2) - 500;
		
		if (wakeUpTime - (endTime - currentTime) < 0) {
			topY = 0;
		}
		
		Vec2 tl(inst.getFactor(laneNum).slope * topY + inst.getFactor(laneNum).intercept + 1, topY);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * topY + inst.getFactor(laneNum + 1).intercept - 1, topY);
		Vec2 bl(inst.getFactor(laneNum).slope * bottomY + inst.getFactor(laneNum).intercept + 1, bottomY);
		Vec2 br(inst.getFactor(laneNum + 1).slope * bottomY + inst.getFactor(laneNum + 1).intercept - 1, bottomY);
		
		note.set(tl, tr, br, bl);
	}
	
}
