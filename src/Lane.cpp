#include"Lane.h"

Lane::Lane():
    underLine(210, 1000, 1710, 1000),
    laneBegin(960, -200),
    interval(400, 0),
    leftEnd(160,1080)
{
	rightEnd = leftEnd + interval * 4;

	for(auto i:step(5)){
		guide.emplace_back(laneBegin, leftEnd + interval * i);
	}
}

Lane& Lane::getInstance(){
	return *instance;
}

void Lane::create(){
	if (!instance) {
		instance = new Lane;
	}
}

void Lane::destroy(){
	delete instance;
	instance = nullptr;
}

void Lane::update(){}

void Lane::draw(){
	// draw background
    Triangle(laneBegin,leftEnd,rightEnd).draw(Color(65, 65, 65));
    
	// draw guide
	for(auto i:step(5)){
        guide[i].draw(LineStyle::RoundCap, (i == 0 || i == 4) ? 8 : 2, Palette::Orange);
	}
    underLine.draw(LineStyle::RoundCap, 8, Palette::Orange);
}

Lane* Lane::instance = nullptr;
