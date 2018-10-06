//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"

namespace ui {
	
	Note::Note(size_t _laneNum, float _speed):
		laneNum(_laneNum),
		speed(_speed),
		inst(ui::LaneBG::getInstance()){}
	
	void Note::draw() const{
		if(note.p3.y > 0 && note.p0.y < Window::Height()){
			note.draw(Palette::Papayawhip);
		}
	}
	
	
	
	
	HitNote::HitNote(size_t _laneNum, float _judgeTime, float _speed):
		Note(_laneNum, _speed),
		judgeTime(_judgeTime){}
	
	void HitNote::update(double currentTime) {
		float nextY = ui::laneEnd - ((judgeTime - currentTime) * ui::laneEnd * speed);
		float height = 60 * (1. / 1250. * nextY + 0.2);
		
		//speed *= (note.p0.y - height / 2) * (1./2000.) + 0.5;
		
		if (nextY - height/2 < 0) {
			note.set(0,0,0,0,0,0,0,0);
			return;
		}
		
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
		float bottomY = ui::laneEnd - ((startTime - currentTime) * ui::laneEnd * speed);
		
		float topY = ui::laneEnd - ((endTime - currentTime) * ui::laneEnd * speed);
		
		//speed *= note.p0.y * (1./2000.) + 0.5;
		
		if (bottomY < 0) {
			note.set(0, 0, 0, 0, 0, 0, 0, 0);
			return;
		}
		
		if(topY < 0) {
			topY = 0;
		}
		
		Vec2 tl(inst.getFactor(laneNum).slope * topY + inst.getFactor(laneNum).intercept + 1, topY);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * topY + inst.getFactor(laneNum + 1).intercept - 1, topY);
		Vec2 bl(inst.getFactor(laneNum).slope * bottomY + inst.getFactor(laneNum).intercept + 1, bottomY);
		Vec2 br(inst.getFactor(laneNum + 1).slope * bottomY + inst.getFactor(laneNum + 1).intercept - 1, bottomY);
		
		note.set(tl, tr, br, bl);
	}
	
}
