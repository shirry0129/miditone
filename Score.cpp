#include "Score.hpp"
#include "ScoreReader.hpp"

#include <fstream>
#include <array>


namespace score {


	Score::Score() {}

	Score::~Score() {}

	Status Score::load(const std::string & file, Difficulty diff) {
	



	}

	const std::vector<Note>& Score::getNotes() const noexcept {
		return notes;
	}

	

	bool Score::readSection(const std::ifstream & stream, char * buf, size_t bufSize, size_t section) {
		
		stream.getline()
		
		return false;
	}

	bool Score::skip(const std::ifstream & stream, char destFront) {
		return false;
	}

	bool Score::read(const std::ifstream &stream, , size_t charCnt) {
		
		stream.get

		return false;
	}


	bool Score::



}