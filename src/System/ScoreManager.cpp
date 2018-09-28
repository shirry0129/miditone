#include "ScoreManager.hpp"
#include "ScoreTimeConverter.hpp"

namespace score {

	ScoreManager::ScoreManager() noexcept {}

	ScoreManager::ScoreManager(const char *file, Difficulty difficulty) {
		create(file, difficulty);
	}

	ScoreManager::~ScoreManager() {}

	bool ScoreManager::create(const char *file, Difficulty difficulty) {
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

		double sec1 = 0.0f;
		double sec2 = 0.0f;

		int hitCount = 0;
		int holdCount = 0;

		for (const auto &e : event) {

			if (e.lane < 0 || e.lane > 3)
				return false;	// invalied lane


			switch (e.type) {
			case 0:
				break;
			case 1: // hit
				sec1 = timeConv.calcSec(e.getBarLength());

				notes.emplace_back(
					Note(NoteType::HIT, e.lane, hitCount + holdCount, 
						NoteTime(e.bar, sec1), NoteTime(e.bar, sec1))
				);

				hitCount++;

				break;
			case 2: // hold begin
				lastHoldBegin.at(e.lane) = &e;
				break;
			case 3: // hold end
			{
				const auto begin = lastHoldBegin.at(e.lane);
				sec1 = timeConv.calcSec(begin->getBarLength());
			}
				sec2 = timeConv.calcSec(e.getBarLength());

				notes.emplace_back(
					Note(NoteType::HOLD, e.lane, hitCount + holdCount, 
						NoteTime(e.bar, sec1), NoteTime(e.bar, sec2))
				);

				lastHoldBegin.at(e.lane) = nullptr;  // end reference
				holdCount++;

				break;
			default:
				// invalied note type
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
		for (int i = 1; i <= numofBars + 1; i++) {
			bar.emplace_back(
				NoteTime(i, timeConv.calcSec(ScoreTime(i).getBarLength())), i
			);
		}

		
		return true;
	}

	bool ScoreManager::recreate() {
		return create(path.c_str(), header.difficulty);
	}

	void ScoreManager::clear() {
		init();
	}

	int ScoreManager::getNumofBars() const noexcept {
		return numofBars;
	}

	int ScoreManager::getNumofHolds() const noexcept {
		return numofHolds;
	}

	int ScoreManager::getNumofHits() const noexcept {
		return numofHits;
	}

	const std::vector<Tempo>& ScoreManager::getTempo() const noexcept {
		return tempo;
	}

	float ScoreManager::getTempo(double sec) const noexcept {
		for (auto it = tempo.crbegin(); it != tempo.crend(); it++) {
			if (it->time.sec <= sec)
				return it->tempo;
		}

		return 0.0f;
	}

	const std::vector<Beat>& ScoreManager::getBeat() const noexcept {
		return beat;
	}

	math::Fraction ScoreManager::getBeat(double sec) const noexcept {
		for (auto it = beat.crbegin(); it != beat.crend(); it++) {
			if (it->time.sec <= sec)
				return it->beat;
		}

		return math::Fraction(0);
	}

	const std::vector<Bar>& ScoreManager::getBar() const noexcept {
		return bar;
	}

	int ScoreManager::getBar(double sec) const noexcept {
		for (auto it = bar.crbegin(); it != bar.crend(); it++) {
			if (it->time.sec <= sec)
				return it->barCnt;
		}

		return 0;
	}

	const ScoreManager::Header & ScoreManager::getHeader() const noexcept {
		return header;
	}

	const std::vector<Note> & ScoreManager::getNotes() const noexcept {
		return notes;
	} 

	const score::ScoreTimeConverter &ScoreManager::getConverter() const noexcept {
		return timeConv;
	}

	void ScoreManager::init() {
		notes.clear();
		path.clear();
		timeConv.clear();
		numofBars = 0;
		numofHolds = 0;
		numofHits = 0;
		header.clear();
		tempo.clear();
		beat.clear();
		bar.clear();
	}


	

}