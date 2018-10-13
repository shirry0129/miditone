//
//  JudgeEffect.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/10.
//

#ifndef JudgeEffect_hpp
#define JudgeEffect_hpp


#include <Siv3D.hpp>

#include "Lane.hpp"


namespace ui {

	void writeShineImage(
		Image &image, int radiusOut = 100, int radiusIn = 15, int blurSize = 15,
		const Color& color = Palette::White
	);
	

	struct JudgeEffect : IEffect {
	public:
		static constexpr int diamondRotateDeg = 720;
		static constexpr int orbitRotateDeg = 40;
		
		JudgeEffect(
			const Texture &shineTexture, double _x, double _y, double _remainSec,
			int radius, int _shineDeg, const Color& color, bool _hasDrawShine = true
		) noexcept;
		
		JudgeEffect(
			const Texture &shineTexture, const Point& pos, double _remainSec,
			int radius, int _shineDeg, const Color& color, bool _hasDrawShine = true
		) noexcept;
	
		virtual ~JudgeEffect();
		
		virtual bool update(double t) override;
		
	private:
		const Texture& shineTexture;
		const int x;
		const int y;
		const double remainSec;
		const double convexPeakSec;
		const double rotatePauseSec;
		const double shineRad;
		const int shineRadius;
		const Color color;
		const bool hasDrawShine;
	};
	
	
	
	struct _1stJudgeEffect : JudgeEffect {
	public:
		static constexpr int radius = 250;
		
		_1stJudgeEffect(
			const Texture &shineTexture, double _x, double _y,
			double _remainSec, bool _hasDrawShine = true
		) noexcept;
		
		_1stJudgeEffect(
			const Texture &shineTexture, const Point& pos,
			double _remainSec, bool _hasDrawShine = true
		) noexcept;
		
		~_1stJudgeEffect();
		
	};
	
	
	struct _2ndJudgeEffect : JudgeEffect {
	public:
		static constexpr int radius = 200;

		_2ndJudgeEffect(
			const Texture &shineTexture, double _x, double _y,
			double _remainSec, bool _hasDrawShine = true
		) noexcept;
		
		_2ndJudgeEffect(
			const Texture &shineTexture, const Point& pos,
			double _remainSec, bool _hasDrawShine = true
		) noexcept;
		
		~_2ndJudgeEffect();
		
	};
	
	
	struct _3rdJudgeEffect : JudgeEffect {
	public:
		static constexpr int radius = 150;

		_3rdJudgeEffect(
			const Texture &shineTexture, double _x, double _y,
			double _remainSecx, bool _hasDrawShine = true
		) noexcept;
		
		_3rdJudgeEffect(
			const Texture &shineTexture, const Point& pos,
			double _remainSec, bool _hasDrawShine = true
		) noexcept;
		
		~_3rdJudgeEffect();
		
	};
	
	
	
	
	
	using CriticalHitEffect = _1stJudgeEffect;
	using CorrectHitEffect = _2ndJudgeEffect;
	using NiceHitEffect = _3rdJudgeEffect;
	

}


#endif /* JudgeEffect_hpp */
