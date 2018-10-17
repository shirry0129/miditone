//
//  JudgeStrEffect.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/17.
//

#include "JudgeStrEffect.hpp"



namespace ui {

	JudgeStrEffect::JudgeStrEffect(
		const Font& _font, const String& _str, const Vec2& _fromPos, double _remainSec, const Color& _color
	) noexcept
	: judgeFont(_font)
    , str(_str)
	, fromPos(_fromPos)
	, remainSec(_remainSec)
	, color(_color) {}

	bool JudgeStrEffect::update(double t) {
		if (t >= remainSec)
			return false;
		
		const double progress = t/remainSec;
//		const double invProgress = 1.0 - progress;
		
		const int alpha = static_cast<int>(255 * progress);
		const double height = floatingHeight * progress;


	    judgeFont(str).drawAt(fromPos - Vec2(0, height), Color(color, alpha));

		return true;
	}
	
	
	
	_1stJudgeStrEffect::_1stJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(_font, U"CRITICAL", _fromPos, _remainSec, HSV(60, 0.7, 1.0)) {};
	
	
	_2ndJudgeStrEffect::_2ndJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(_font, U"CORRECT", _fromPos, _remainSec, Palette::Hotpink) {};
	
	
	_3rdJudgeStrEffect::_3rdJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(_font, U"NICE", _fromPos, _remainSec, Palette::Skyblue) {};
	
	
	MissJudgeStrEffect::MissJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec
	) noexcept
	: JudgeStrEffect(_font, U"MISS", _fromPos, _remainSec, Palette::Lightgrey) {};
	
	

}
