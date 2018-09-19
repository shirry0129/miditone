#include "Song.hpp"



namespace song {

	Song::Song() {
	

	}


	Song::~Song() {}


	Song::Header::Header(std::string _title, std::string _artist, Genre _genre) 
		: title(_title), artist(_artist), genre(_genre) {
	}

	Song::Header::~Header() {}

	Song::TickTime::TickTime(int barCnt, const math::Fraction & posInBar) {
		set(barCnt, posInBar);
	}

	Song::TickTime::~TickTime() {
	}

}