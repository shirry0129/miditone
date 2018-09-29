//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"
#include "Lane.hpp"

namespace ui {
	
	void Note::hitLocate(double currentTime) {
		ui::LaneBG& inst = ui::LaneBG::getInstance();
		
		float nextY = ui::laneEnd - ((startTime - currentTime) * ui::laneEnd * speed);
		
		Vec2 tl(inst.getFactor(laneNum).slope * nextY + inst.getFactor(laneNum).intercept + 1, nextY - 5);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * nextY + inst.getFactor(laneNum + 1).intercept - 1, nextY - 5);
		Vec2 bl(inst.getFactor(laneNum).slope * nextY + inst.getFactor(laneNum).intercept + 1, nextY + 5);
		Vec2 br(inst.getFactor(laneNum + 1).slope * nextY + inst.getFactor(laneNum + 1).intercept - 1, nextY + 5);
		
		note.set(tl, tr, br, bl);;
	}
	
	void Note::holdLocate(double currentTime) {
		ui::LaneBG& inst = ui::LaneBG::getInstance();
		
		float bottomY = ui::laneEnd - ((startTime - currentTime) * ui::laneEnd * speed);
		float topY = ui::laneEnd - ((endTime - currentTime) * ui::laneEnd * speed);
		
		if(topY < 0) {
			topY = 0;
		}
		
		Vec2 tl(inst.getFactor(laneNum).slope * topY + inst.getFactor(laneNum).intercept + 1, topY);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * topY + inst.getFactor(laneNum + 1).intercept - 1, topY);
		Vec2 bl(inst.getFactor(laneNum).slope * bottomY + inst.getFactor(laneNum).intercept + 1, bottomY);
		Vec2 br(inst.getFactor(laneNum + 1).slope * bottomY + inst.getFactor(laneNum + 1).intercept - 1, bottomY);
		
		note.set(tl, tr, br, bl);;
	}
	
	Note::Note(int _laneNum, float _startTime, float _speed):
		laneNum(_laneNum),
		startTime(_startTime),
		endTime(0),
		speed(_speed),
		nType(score::NoteType::HIT){}
	
	Note::Note(int _laneNum, float _startTime, float _endTime, float _speed):
		laneNum(_laneNum),
		startTime(_startTime),
		endTime(_endTime),
		speed(_speed),
		nType(score::NoteType::HOLD){}
	
	void Note::update(double currentTime) {
		switch (nType) {
			case score::NoteType::HIT:
				hitLocate(currentTime);
				break;
				
			case score::NoteType::HOLD:
				holdLocate(currentTime);
				break;
		};
	}
	
	void Note::draw() {
		if(note.p3.y > 0 && note.p0.y < Window::Height()){
			note.draw();
		}
	}
	
}
