#include "ScoreManager.hpp"
#include "ScoreTimeConverter.hpp"

#include <list>

namespace score {

	ScoreManager::ScoreManager() noexcept
		: prevError(State::S_OK, createErrMessage) {}

	ScoreManager::ScoreManager(const std::string &file, Difficulty difficulty)
		: prevError(State::S_OK, createErrMessage) {
		create(file, difficulty);
	}

	ScoreManager::~ScoreManager() {}

	score_err_t ScoreManager::create(const std::string &file, Difficulty difficulty) {
		using namespace score;

		init();

		reader.open(file.c_str());

		// read header
		score::Header h;
		
		if (reader.readHeader(h, "header").isError())
			return prevError = State::E_READER_FAILED;


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
			return prevError = State::E_INVALID_ARGUMENT;
		}


		// read note data
		std::vector<NoteEvent> event;
		
		if (reader.readNote(event, chunkName).isError())
			return prevError = State::E_READER_FAILED;


		// create note timing data
		std::array<const NoteEvent*, numofLanes> lastHoldBegin;

		if (!timeConv.create(h.beat, h.tempo))
			return prevError = State::E_INVALID_TEMPO_BEAT;
		

		double sec1 = 0.0f;
		double sec2 = 0.0f;

		int hitCount = 0;
		int holdCount = 0;
		std::array<int, numofLanes> laneNoteCnt;
		for (auto &n : laneNoteCnt)
			n = 0;
		
		std::list<Note> unorderedNote;
	
		for (const auto &e : event) {

			if (e.lane < 0 || e.lane > 3)
				return prevError = State::E_INVALID_LANE;	// invalied lane


			switch (e.type) {
			case 0:
				break;
			case 1: // hit
				sec1 = timeConv.calcSec(e.getBarLength());

				unorderedNote.emplace_back(
					NoteType::HIT, e.lane, hitCount + holdCount, laneNoteCnt.at(e.lane),
					NoteTime(e.bar, sec1), NoteTime(e.bar, sec1)
				);

				hitCount++;
				laneNoteCnt.at(e.lane)++;

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

				unorderedNote.emplace_back(
					NoteType::HOLD, e.lane, hitCount + holdCount, laneNoteCnt.at(e.lane),
					NoteTime(e.bar, sec1), NoteTime(e.bar, sec2)
				);

				lastHoldBegin.at(e.lane) = nullptr;  // end reference
				holdCount++;
				laneNoteCnt.at(e.lane)++;

				break;
			default:
				// invalied note type
				return prevError = State::E_INVALID_NOTE;
			}
		}
		
		
		// sort
		unorderedNote.sort(
			[](const Note& a, const Note&b) { return a.t_beg.sec < b.t_beg.sec; }
		);
	
	
		// add notes
		hitCount = 0;
		holdCount = 0;
		for (auto &n : laneNoteCnt)
			n = 0;
		
		notes.reserve(hitCount + holdCount);
		for (const auto& n : unorderedNote) {
			notes.emplace_back(
				n.type, n.lane, hitCount + holdCount, laneNoteCnt.at(n.lane),
				n.t_beg, n.t_end
			);
			
			switch (n.type) {
			  case NoteType::HIT: hitCount++; break;
			  case NoteType::HOLD: holdCount++; break;
			}
			
			laneNoteCnt.at(n.lane)++;
		}
		
		
		// update member variable
		numofBars = event.back().bar;
		numofHits = hitCount;
		numofHolds = holdCount;
		numofNotesInLane = laneNoteCnt;
		path.assign(file);

		header.difficulty = difficulty;
		header.title = h.title;
		header.artist = h.artist;
		header.level = h.level.at(static_cast<int>(difficulty));
		if (h.genre == "0")
			header.genre = Genre::JPOP_ANIME;
		else if (h.genre == "1")
			header.genre = Genre::GAME;
		else if (h.genre == "2")
			header.genre = Genre::NICONICO;
		else if (h.genre == "3")
			header.genre = Genre::VARIETY;
	

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

		
		return prevError = State::S_OK;
	}

	score_err_t ScoreManager::recreate() {
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
	
	int ScoreManager::getNumofNotes() const noexcept {
		return numofHits + numofHolds;
	}
	
	int ScoreManager::getNumofLaneNotes(int laneNum) const noexcept {
		if (laneNum < 0 || laneNum >= numofLanes)
			return 0;
		
		return numofNotesInLane.at(laneNum);
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
	
	const Error<ScoreManager::State> &ScoreManager::getLastError() const noexcept {
		return prevError;
	}
	
	const ScoreReader &ScoreManager::getReader() const noexcept {
		return reader;
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
		prevError = State::S_OK;
		for (auto &n : numofNotesInLane)
			n = 0;
	}


	std::string ScoreManager::createErrMessage(State state) {
		std::string msg;
	
		switch (state) {
		  case State::S_OK:
			msg += "成功";
			return msg;
		  case State::E_INVALID_ARGUMENT:
		  	msg += "無効な引数です";
		  	return msg;
		  case State::E_INVALID_NOTE:
		  	msg += "無効なノーツが存在します";
		  	return msg;
		  case State::E_READER_FAILED:
		  	msg += "譜面の読み取りに失敗しました";
		  	return msg;
		  case State::E_INVALID_TEMPO_BEAT:
		  	msg += "テンポまたは拍子が無効です";
		  	return msg;
		  case State::E_INVALID_LANE:
		    msg += "無効なレーンが存在します";
		    return msg;
		  default:
		  	return msg;
		}
	}

	

}
