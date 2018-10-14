//
//  JudgeEffect.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/10.
//

#include "JudgeEffect.hpp"


namespace ui {

	void writeShineImage(
		Image &image, int radiusOut, int radiusIn, int blurSize, const Color& color
	) {

		const Color backColor(0, 0, 0, 255);

		image.resize(
			(radiusOut + radiusIn)*2 + blurSize/2,
			(radiusOut + radiusIn)*2 + blurSize/2,
			backColor
		);
		
		Shape2D::NStar(
			4, radiusOut, radiusOut/10, Vec2(image.width()/2.0, image.height()/2.0)
		).asPolygon().paint(image, Color(color, 255));
		
		
		// remove back color
		for (auto &px : image) {
			if (px == backColor)
				px.a = 0;
		}

		image.gaussianBlur(blurSize);
	}



	JudgeEffect::JudgeEffect(
		const Texture &shineTexture, double _x, double _y, double _remainSec,
		int radius, int _shineDeg, const Color& color, bool _hasDrawShine
	) noexcept
	: shineTexture(shineTexture)
	, x(_x)
	, y(_y)
	, remainSec(_remainSec)
	, convexPeakSec(_remainSec * 0.4)
	, rotatePauseSec(_remainSec * 0.2)
	, shineRad(ToRadians(_shineDeg))
	, shineRadius(radius)
	, color(color)
	, hasDrawShine(_hasDrawShine) {};
	
	JudgeEffect::JudgeEffect(
		const Texture &shineTexture, const Point& pos, double _remainSec,
		int radius, int _shineDeg, const Color& color, bool _hasDrawShine
	) noexcept
	: JudgeEffect(
		shineTexture, pos.x, pos.y, _remainSec, radius, _shineDeg, color, _hasDrawShine
	) {}


	JudgeEffect::~JudgeEffect() {};
	
	
	bool JudgeEffect::update(double t) {
		if (t >= remainSec)
			return false;
		
		const double progress = t/remainSec;
		const double quickProgress =
			(t >= remainSec - rotatePauseSec) ? 1.0 : t/(remainSec - rotatePauseSec);
		const double convexProgress =
			(t < convexPeakSec) ?
			t/convexPeakSec :
			1.0 - (t - convexPeakSec)/(remainSec - convexPeakSec);
		
		const double diamondRad = ToRadians(diamondRotateDeg * quickProgress);
		const double orbitRad = ToRadians(orbitRotateDeg * quickProgress);
		const int alpha = 255 * (1.0 - progress);
		const int convexAlpha = 255 * convexProgress;
		const Size diamondSize(shineRadius/48, shineRadius/9);
		const double centerOrbitRadius = shineRadius/4 - diamondSize.y/2;
		const Size ellipseOrbitSize(shineRadius/4, shineRadius/8);
	
	
		// change blend state
		Graphics2D::SetBlendState(BlendState::Additive);
	
		
		// change origin to center of the effect
		const Transformer2D effectCenter(Mat3x2::Translate(x, y));
	
	
		// draw orbit
		for (int i = 0; i < 2; i++) {
			Circle(centerOrbitRadius).drawFrame(1, 0, Color(color, convexAlpha));
			const Transformer2D rotate(Mat3x2::Rotate(orbitRad));
			Ellipse(ellipseOrbitSize.x, ellipseOrbitSize.y).
			drawFrame(1, 0, Color(color, convexAlpha));
		}
		
	
		// draw diamonds
		// inside
		for (int i = 0; i < 8; i++) {
			const Transformer2D orbit(
				Mat3x2::Translate(centerOrbitRadius*0.6, 0)
				.rotated(30_deg - diamondRad/2 + 45_deg*i)
			);
			
			Shape2D::Rhombus(diamondSize.x*0.6, diamondSize.y*0.6, Vec2(0, 0), 90_deg)
			.asPolygon().draw(Color(color, convexAlpha));
		}
		
		// outside
		for (int i = 0; i < 4; i++) {
			const Transformer2D orbit(
				Mat3x2::Translate(centerOrbitRadius, 0)
				.rotated(diamondRad + 90_deg*i)
			);
			
			Shape2D::Rhombus(diamondSize.x, diamondSize.y, Vec2(0, 0), 90_deg)
			.asPolygon().draw(Color(color, convexAlpha));
		}
		
		
		// draw shine
		if (hasDrawShine) {
			const Transformer2D rotate(Mat3x2::Rotate(shineRad));
			
			shineTexture
			.resized(shineRadius*1.0, shineRadius*1.0).rotated(0)
			.drawAt(0, 0, Color(color, alpha));
			
			shineTexture
			.resized(shineRadius*0.4, shineRadius*0.4).rotated(5)
			.drawAt(1, 1, Color(color, alpha));
			
			shineTexture
			.resized(shineRadius*0.6, shineRadius*0.6).rotated(-40)
			.drawAt(-1, -1, Color(color, alpha));
		}
        
        Graphics2D::SetBlendState(BlendState::NonPremultiplied);
		
		
		return true;
	};
	



	_1stJudgeEffect::_1stJudgeEffect(
		const Texture &shineTexture, double _x, double _y,
		double _remainSec, bool _hasDrawShine
	) noexcept
	: JudgeEffect(
		shineTexture, _x, _y, _remainSec, radius, 45, HSV(60, 0.7, 1.0), _hasDrawShine
	) {};
	
	
	_1stJudgeEffect::_1stJudgeEffect(
		const Texture &shineTexture, const Point& pos, double _remainSec, bool _hasDrawShine
	) noexcept
	: _1stJudgeEffect(
		shineTexture, pos.x, pos.y, _remainSec, _hasDrawShine
	) {}
	
	_1stJudgeEffect::~_1stJudgeEffect() {};
	
	

	_2ndJudgeEffect::_2ndJudgeEffect(
		const Texture &shineTexture, double _x, double _y, double _remainSec, bool _hasDrawShine
	) noexcept
	: JudgeEffect(
		shineTexture, _x, _y, _remainSec, radius, 45, Palette::Hotpink, _hasDrawShine
	) {};
	
	
	_2ndJudgeEffect::_2ndJudgeEffect(
		const Texture &shineTexture, const Point& pos, double _remainSec, bool _hasDrawShine
	) noexcept
	: _2ndJudgeEffect(
		shineTexture, pos.x, pos.y, _remainSec, _hasDrawShine
	) {}
	
	_2ndJudgeEffect::~_2ndJudgeEffect() {};



	
	_3rdJudgeEffect::_3rdJudgeEffect(
		const Texture &shineTexture, double _x, double _y, double _remainSec, bool _hasDrawShine
	) noexcept
	: JudgeEffect(
		shineTexture, _x, _y, _remainSec, radius, 45, Palette::Skyblue, _hasDrawShine
	) {};
	
	
	_3rdJudgeEffect::_3rdJudgeEffect(
		const Texture &shineTexture, const Point& pos, double _remainSec, bool _hasDrawShine
	) noexcept
	: _3rdJudgeEffect(
		shineTexture, pos.x, pos.y, _remainSec, _hasDrawShine
	) {}
	
	_3rdJudgeEffect::~_3rdJudgeEffect() {};
	
	
	
}
