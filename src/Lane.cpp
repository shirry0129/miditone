#include"Lane.hpp"

Lane::LaneBG::LaneBG():
    underLine(210, 1000, 1710, 1000),
    laneBegin(960, -200),
    interval(400, 0),
    leftEnd(160,1080),
    rightEnd(leftEnd + interval * 4)
{
	for(auto i:step(5)){
		guide.emplace_back(laneBegin, leftEnd + interval * i);
	}
}

Lane::LaneBG& Lane::LaneBG::getInstance(){
	return *instance;
}

void Lane::LaneBG::create(){
	if (!instance) {
		instance = new LaneBG;
	}
}

void Lane::LaneBG::destroy(){
	delete instance;
    instance = nullptr;
}

void Lane::LaneBG::update(){}

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
