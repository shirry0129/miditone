//
//  JudgeStrEffect.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/17.
//

#include "JudgeStrEffect.hpp"



namespace ui {

	JudgeStrEffect::JudgeStrEffect(
		const Font& _font, const String& _str, const Vec2& _fromPos, double _remainSec, const Color& _color, double _angle
	) noexcept
	: judgeFont(_font)
    , str(_str)
	, fromPos(_fromPos)
	, remainSec(_remainSec)
	, color(_color)
    , angle(_angle){}

	bool JudgeStrEffect::update(double t) {
		if (t >= remainSec)
			return false;
		
		const double progress = t/remainSec;
		
		const int alpha = static_cast<int>(255 * progress);
		const double height = floatingHeight * progress;

        {
            Transformer2D r(Mat3x2::Rotate(angle, fromPos + Vec2(0, floatingHeight)));
            judgeFont(str).drawAt(fromPos - Vec2(0, height), Color(color, alpha));
        }

		return true;
	}
	
	
	
	_1stJudgeStrEffect::_1stJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
	) noexcept
	: JudgeStrEffect(_font, U"CRITICAL", _fromPos, _remainSec, HSV(60, 0.7, 1.0), _angle) {};
	
	
	_2ndJudgeStrEffect::_2ndJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
	) noexcept
	: JudgeStrEffect(_font, U"CORRECT", _fromPos, _remainSec, Palette::Hotpink, _angle) {};
	
	
	_3rdJudgeStrEffect::_3rdJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
	) noexcept
	: JudgeStrEffect(_font, U"NICE", _fromPos, _remainSec, Palette::Skyblue, _angle) {};
	
	
	MissJudgeStrEffect::MissJudgeStrEffect(
		const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
	) noexcept
	: JudgeStrEffect(_font, U"MISS", _fromPos, _remainSec, Palette::Lightgrey, _angle) {};
	
	

}
