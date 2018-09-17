#include"Lane.h"

Lane::Lane() {
	underLine = Line(0, 1000, 1920, 1000);
	laneBegin = Vec2(960, -200);
	interval = Vec2(400,0);
	leftEnd = Vec2(160,1000);
	rightEnd = leftEnd + interval * 4;

	for(auto i:step(5)){
		guide.emplace_back(laneBegin, leftEnd + interval * i);
	}
}

Lane::~Lane(){
	guide.clear();
	guide.shrink_to_fit();
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
	for(auto l:guide){
		l.draw(2, Palette::Orange);
	}
	underLine.draw(8, Palette::Orange);
}

Lane* Lane::instance = nullptr;
