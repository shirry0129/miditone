#include "Score.hpp"
#include "ScoreTimeConverter.hpp"

namespace score {

	Score::Score() noexcept {}

	Score::Score(const char *file, Difficulty difficulty) {
		create(file, difficulty);
	}

	Score::~Score() {}

	bool Score::create(const char *file, Difficulty difficulty) {
		using namespace score;

		init();


		ScoreReader reader(file);

		// read header
		score::Header h;
		if (ScoreReader::failed(reader.readHeader(h, "header")))
			return false;


		// create chunk name of difficulty
		std::basic_string<char_type> chunkName;

		switch (difficulty) {
		case score::Difficulty::EASY:
			chunkName = "easy";
			break;
		case score::Difficulty::NORMAL:
			chunkName = "normal";
			break;
		case score::Difficulty::HARD:
			chunkName = "hard";
			break;
		default:
			return false;
		}


		// read note data
		std::vector<NoteEvent> event;
		if (ScoreReader::failed(reader.readNote(event, chunkName)))
			return false;


		// create note timing data
		std::array<const NoteEvent*, numofLanes> lastHoldBegin;

		if (!timeConv.create(h.beat, h.tempo))
			return false;

		double msec1 = 0.0f;
		double msec2 = 0.0f;

		int hitCount = 0;
		int holdCount = 0;

		for (const auto &e : event) {

			switch (e.type) {
			case 0:
				break;
			case 1: // hit
				msec1 = timeConv.calcSec(e.getBarLength());

				notes.emplace_back(
					Note(NoteType::HIT, e.lane, hitCount + holdCount, 
						NoteTime(e.bar, msec1), NoteTime(e.bar, msec1))
				);

				hitCount++;

				break;
			case 2: // hold begin
				lastHoldBegin.at(e.lane) = &e;
				break;
			case 3: // hold end
			{
				const auto begin = lastHoldBegin.at(e.lane);
				msec1 = timeConv.calcSec(begin->getBarLength());
			}
				msec2 = timeConv.calcSec(e.getBarLength());

				notes.emplace_back(
					Note(NoteType::HOLD, e.lane, hitCount + holdCount, 
						NoteTime(e.bar, msec1), NoteTime(e.bar, msec2))
				);

				lastHoldBegin.at(e.lane) = nullptr;  // end reference
				holdCount++;

				break;
			default:
				return false;
			}
		}


		// update member variable
		numofBars = event.back().bar;
		numofHits = hitCount;
		numofHolds = holdCount;
		path.assign(file);

		header.difficulty = difficulty;
		header.title = h.title;
		header.artist = h.artist;
		header.level = h.level.at(static_cast<int>(difficulty));

		for (const auto &t : h.tempo) {
			tempo.emplace_back(
				NoteTime(t.bar, timeConv.calcSec(t.getBarLength())), t.tempo
			);
		}
		for (const auto &b : h.beat) {
			beat.emplace_back(
				NoteTime(b.bar, timeConv.calcSec(b.getBarLength())), b.beat
			);
		}

		
		return true;
	}

	bool Score::recreate() {
		return create(path.c_str(), header.difficulty);
	}

	void Score::clear() {
		init();
	}

	int Score::getNumofBars() const noexcept {
		return numofBars;
	}

	int Score::getNumofHolds() const noexcept {
		return numofHolds;
	}

	int Score::getNumofHits() const noexcept {
		return numofHits;
	}

	const std::vector<Tempo>& Score::getTempo() const noexcept {
		return tempo;
	}

	float Score::getTempo(double msec) const noexcept {
		for (auto it = tempo.crbegin(); it != tempo.crend(); it++) {
			if (it->time.msec <= msec)
				return it->tempo;
		}

		return 0.0f;
	}

	const std::vector<Beat>& Score::getBeat() const noexcept {
		return beat;
	}

	math::Fraction Score::getBeat(double msec) const noexcept {
		for (auto it = beat.crbegin(); it != beat.crend(); it++) {
			if (it->time.msec <= msec)
				return it->beat;
		}

		return math::Fraction(0);
	}

	const Score::Header & Score::getHeader() const noexcept {
		return header;
	}

	const score::ScoreTimeConverter &Score::getConverter() const noexcept {
		return timeConv;
	}

	void Score::init() {
		notes.clear();
		path.clear();
		timeConv.clear();
		numofBars = 0;
		numofHolds = 0;
		numofHits = 0;
		header.clear();
		tempo.clear();
		beat.clear();
	}


	

}