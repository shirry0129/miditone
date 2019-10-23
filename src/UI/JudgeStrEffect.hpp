//
//  JudgeStrEffect.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/17.
//

#ifndef JudgeStrEffect_hpp
#define JudgeStrEffect_hpp


#include <Siv3D.hpp>


namespace ui {

	
	struct JudgeStrEffect : IEffect {
	
		static constexpr double floatingHeight = 100;

		JudgeStrEffect(
			const Font& _font, const String& _str, const Vec2& _fromPos, double _remainSec, const Color& _color, double _angle
		) noexcept;

		virtual bool update(double t) override;
		
		
	private:
        const Font judgeFont;
		const String str;
		const Vec2 fromPos;
		const double remainSec;
		const Color color;
        const double angle;
		
	};
	
	
	
	struct _1stJudgeStrEffect : JudgeStrEffect {
	
		_1stJudgeStrEffect(
			const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
		) noexcept;

	};
	
	
	struct _2ndJudgeStrEffect : JudgeStrEffect {
	
		_2ndJudgeStrEffect(
			const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
		) noexcept;

	};
	
	
	struct _3rdJudgeStrEffect : JudgeStrEffect {
	
		_3rdJudgeStrEffect(
			const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
		) noexcept;

	};
	
	struct MissJudgeStrEffect : JudgeStrEffect {
	
		MissJudgeStrEffect(
			const Font& _font, const Vec2& _fromPos, double _remainSec, double _angle
		) noexcept;

	};
	
	
	
	using CriticalStrEffect = _1stJudgeStrEffect;
	using CorrectStrEffect = _2ndJudgeStrEffect;
	using NiceStrEffect = _3rdJudgeStrEffect;
	using MissStrEffect = MissJudgeStrEffect;

}


#endif /* JudgeStrEffect_hpp */
