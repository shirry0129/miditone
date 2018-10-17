//
//  JudgeStrEffect.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/17.
//

#include "JudgeStrEffect.hpp"



namespace ui {

	JudgeStrEffect::JudgeStrEffect(
		const String& _str, const Vec2& _fromPos, double _remainSec, const Color& _color
	) noexcept
	: str(_str)
	, fromPos(_fromPos)
	, remainSec(_remainSec)
	, color(_color) {}

	bool JudgeStrEffect::update(double t) {
		if (t >= remainSec)
			return false;
		
		const double progress = t/remainSec;
		const double invProgress = 1.0 - progress;
		
		const int alpha = static_cast<int>(255 * invProgress);
		const double height = floatingHeight * progress;


	    FontAsset(U"JudgeFont")(str).drawAt(fromPos - Vec2(0, height), Color(color, alpha));

		return true;
	}
	
	
	
	_1stJudgeStrEffect::_1stJudgeStrEffect(
		const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(U"CRITICAL", _fromPos, _remainSec, HSV(60, 0.7, 1.0)) {};
	
	
	_2ndJudgeStrEffect::_2ndJudgeStrEffect(
		const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(U"CORRECT", _fromPos, _remainSec, Palette::Hotpink) {};
	
	
	_3rdJudgeStrEffect::_3rdJudgeStrEffect(
		const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(U"NICE", _fromPos, _remainSec, Palette::Skyblue) {};
	
	
	MissJudgeStrEffect::MissJudgeStrEffect(
		const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(U"MISS", _fromPos, _remainSec, Palette::Lightgrey) {};
	
	

}
