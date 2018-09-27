//
//  Notes.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/09/17.
//

#include "Note.hpp"
#include "Lane.hpp"

namespace ui {
	
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
		ui::LaneBG& inst = ui::LaneBG::getInstance();
		
		float nextY = ui::laneEnd - ((startTime - currentTime) * ui::laneEnd * speed);
		float nextX = inst.getFactor(laneNum - 1).slope * nextY + inst.getFactor(laneNum - 1).intercept;
		float nextWidth = inst.getFactor(laneNum).slope * nextY + inst.getFactor(laneNum).intercept - nextX;
		
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
		ui::LaneBG& inst = ui::LaneBG::getInstance();
		
		float bottomY = ui::laneEnd - ((startTime - currentTime) * ui::laneEnd * speed);
		float topY = ui::laneEnd - ((endTime - currentTime) * ui::laneEnd * speed);
		
		if(topY < 0) {
			topY = 0;
		}
		
		Vec2 tl(inst.getFactor(laneNum - 1).slope * topY + inst.getFactor(laneNum - 1).intercept + 1, topY);
		Vec2 tr(inst.getFactor(laneNum).slope * topY + inst.getFactor(laneNum).intercept - 1, topY);
		Vec2 bl(inst.getFactor(laneNum - 1).slope * bottomY + inst.getFactor(laneNum - 1).intercept + 1, bottomY);
		Vec2 br(inst.getFactor(laneNum).slope * bottomY + inst.getFactor(laneNum).intercept - 1, bottomY);
		
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
	
}
