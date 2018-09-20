#include"Lane.hpp"

/**
 Constructor
 */
Lane::LaneBG::LaneBG():
    underLine(210, 1000, 1710, 1000),
    laneBegin(Window::Width()/2, -200),
    interval(400, 0),
    leftEnd(160,Window::Height()),
    rightEnd(leftEnd + interval * 4)
{
	for(auto i:step(5)){
		guide.emplace_back(laneBegin, leftEnd + interval * i);
	}
}

/**
 シングルトンのインスタンスを取得

 @return インスタンスへの参照
 */
Lane::LaneBG& Lane::LaneBG::getInstance(){
	return *instance;
}

/**
 インスタンスの生成
 */
void Lane::LaneBG::create(){
	if (!instance) {
		instance = new LaneBG;
	}
}

/**
 インスタンスの削除
 */
void Lane::LaneBG::destroy(){
	delete instance;
    instance = nullptr;
}

/**
 描画情報の更新
 */
void Lane::LaneBG::update(){}

/**
 レーンとガイドを描画
 */
void Lane::LaneBG::draw(){
	// draw background
    Triangle(laneBegin,leftEnd,rightEnd).draw(Color(65, 65, 65));
    
	// draw guide
	for(auto i:step(5)){
        guide[i].draw(LineStyle::RoundCap, (i == 0 || i == 4) ? 8 : 2, Palette::Orange);
	}
    underLine.draw(LineStyle::RoundCap, 8, Palette::Orange);
}

Lane::LaneBG* Lane::LaneBG::instance = nullptr;
