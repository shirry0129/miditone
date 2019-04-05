//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"
#include "../GameInfo.hpp"

namespace ui {
	
	Note::Note(size_t _laneNum, float _wakeUpTime, float _acceleration):
	laneNum(_laneNum),
	wakeUpTime(_wakeUpTime),
	acceleration(_acceleration),
	isJudged(false),
	isJudging(false),
	inst(ui::LaneBG::getInstance()){}
	
	void Note::makeJudged() {
		isJudged = true;
	}
	
	void Note::makeJudging() {
		isJudging = true;
	}
	
	
	
	
	
	HitNote::HitNote(size_t _laneNum, float _judgeTime, float _wakeUpTime, float _acceleration):
	Note(_laneNum, _wakeUpTime, _acceleration),
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
	
	void HitNote::draw() const{
		if (!isJudged) {
			if(note.p3.y > 0 && note.p0.y < ::gameinfo::originalResolution.y){
				note(TextureAsset(U"hitNote")).draw();
			}
		}
	}
	
	
	
	
	HoldNote::HoldNote(size_t _laneNum, float _startTime, float _endTime, float _wakeUpTime, float _acceleration):
	Note(_laneNum, _wakeUpTime, _acceleration),
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
		
		if (isJudging) {
			bottomY = laneEnd;
		}
		
		float startHeight = 60 * (1. / 1250. * bottomY + 0.2);
		float endHeight = 60 * (1. / 1250. * topY + 0.2);
		
		Vec2 tl(inst.getFactor(laneNum).slope * topY + inst.getFactor(laneNum).intercept + 1, topY);
		Vec2 tr(inst.getFactor(laneNum + 1).slope * topY + inst.getFactor(laneNum + 1).intercept - 1, topY);
		Vec2 bl(inst.getFactor(laneNum).slope * bottomY + inst.getFactor(laneNum).intercept + 1, bottomY);
		Vec2 br(inst.getFactor(laneNum + 1).slope * bottomY + inst.getFactor(laneNum + 1).intercept - 1, bottomY);
		
		note.set(tl, tr, br, bl);
		
		tl = Vec2(inst.getFactor(laneNum).slope * (topY - endHeight / 2) + inst.getFactor(laneNum).intercept + 1, topY - endHeight / 2);
		tr = Vec2(inst.getFactor(laneNum + 1).slope * (topY - endHeight / 2) + inst.getFactor(laneNum + 1).intercept - 1, topY - endHeight / 2);
		bl = Vec2(inst.getFactor(laneNum).slope * (topY + endHeight / 2) + inst.getFactor(laneNum).intercept + 1, topY + endHeight / 2);
		br = Vec2(inst.getFactor(laneNum + 1).slope * (topY + endHeight / 2) + inst.getFactor(laneNum + 1).intercept - 1, topY + endHeight / 2);
		
		end.set(tl, tr, br, bl);
		
		tl = Vec2(inst.getFactor(laneNum).slope * (bottomY - startHeight / 2) + inst.getFactor(laneNum).intercept + 1, bottomY - startHeight / 2);
		tr = Vec2(inst.getFactor(laneNum + 1).slope * (bottomY - startHeight / 2) + inst.getFactor(laneNum + 1).intercept - 1, bottomY - startHeight / 2);
		bl = Vec2(inst.getFactor(laneNum).slope * (bottomY + startHeight / 2) + inst.getFactor(laneNum).intercept + 1, bottomY + startHeight / 2);
		br = Vec2(inst.getFactor(laneNum + 1).slope * (bottomY + startHeight / 2) + inst.getFactor(laneNum + 1).intercept - 1, bottomY + startHeight / 2);
		
		start.set(tl, tr, br, bl);
	}
	
	void HoldNote::draw() const {
		if (!isJudged) {
			if(note.p3.y > 0 && note.p0.y < ::gameinfo::originalResolution.y){
				note.draw(Color(U"#7fffd4"));
				start(TextureAsset(U"hitNote")).draw();
			}
			if (note.p0.y > 0) {
				end(TextureAsset(U"hitNote")).draw();
			}
		}
	}
	
	
}
