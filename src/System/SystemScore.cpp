#include "SystemScore.hpp"

#include <list>
#include "ChEncoder.hpp"

namespace score {

	SystemScore::SystemScore() noexcept
		: prevError(State::S_OK, createErrMessage) {}

	SystemScore::SystemScore(const std::basic_string<char_type> &file, Difficulty difficulty)
		: prevError(State::S_OK, createErrMessage) {
		create(file, difficulty);
	}

	SystemScore::~SystemScore() {}

	score_err_t SystemScore::create(const std::basic_string<char_type> &file, Difficulty difficulty) {
		using namespace score;

		init();


		// read header
		score::Header h;
		if (h.read(file).isError())
			return prevError = State::E_READER_FAILED;
		
		
		reader.open(ch_encoder::toUTF8(file).c_str());


		// create chunk name of difficulty
		std::basic_string<ScoreReader::rch_type> chunkName;

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

		if (!timeConv.create(h.beat(), h.tempo()))
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
		header.id = h.id();
		header.title = h.title();
		header.artist = h.artist();
		header.level = h.level().at(static_cast<int>(difficulty));
		header.genre = h.genre();
	

		for (const auto &t : h.tempo()) {
			tempo.emplace_back(
				NoteTime(t.bar, timeConv.calcSec(t.getBarLength())), t.tempo
			);
		}
		for (const auto &b : h.beat()) {
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

	score_err_t SystemScore::recreate() {
		return create(path.c_str(), header.difficulty);
	}

	void SystemScore::clear() {
		init();
	}

	int SystemScore::getNumofBars() const noexcept {
		return numofBars;
	}

	int SystemScore::getNumofHolds() const noexcept {
		return numofHolds;
	}

	int SystemScore::getNumofHits() const noexcept {
		return numofHits;
	}
	
	int SystemScore::getNumofNotes() const noexcept {
		return numofHits + numofHolds;
	}
	
	int SystemScore::getNumofLaneNotes(int laneNum) const noexcept {
		if (laneNum < 0 || laneNum >= numofLanes)
			return 0;
		
		return numofNotesInLane.at(laneNum);
	}
	
	const std::vector<Tempo>& SystemScore::getTempo() const noexcept {
		return tempo;
	}

	float SystemScore::getTempo(double sec) const noexcept {
		for (auto it = tempo.crbegin(); it != tempo.crend(); it++) {
			if (it->time.sec <= sec)
				return it->tempo;
		}

		return 0.0f;
	}

	const std::vector<Beat>& SystemScore::getBeat() const noexcept {
		return beat;
	}

	math::Fraction SystemScore::getBeat(double sec) const noexcept {
		for (auto it = beat.crbegin(); it != beat.crend(); it++) {
			if (it->time.sec <= sec)
				return it->beat;
		}

		return math::Fraction(0);
	}

	const std::vector<Bar>& SystemScore::getBar() const noexcept {
		return bar;
	}

	int SystemScore::getBar(double sec) const noexcept {
		for (auto it = bar.crbegin(); it != bar.crend(); it++) {
			if (it->time.sec <= sec)
				return it->barCnt;
		}

		return 0;
	}

	const SystemScore::Header & SystemScore::getHeader() const noexcept {
		return header;
	}

	const std::vector<Note> & SystemScore::getNotes() const noexcept {
		return notes;
	} 

	const score::ScoreTimeConverter &SystemScore::getConverter() const noexcept {
		return timeConv;
	}
	
	const Error<SystemScore::State> &SystemScore::getLastError() const noexcept {
		return prevError;
	}
	
	const ScoreReader &SystemScore::getReader() const noexcept {
		return reader;
	}
	

	void SystemScore::init() {
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


	std::basic_string<char_type> SystemScore::createErrMessage(State state) {
		switch (state) {
		  case State::S_OK:
			return U"成功";
		  case State::E_INVALID_ARGUMENT:
		  	return (U"無効な引数です");
		  case State::E_INVALID_NOTE:
		  	return (U"無効なノーツが存在します");
		  case State::E_READER_FAILED:
		  	return (U"譜面の読み取りに失敗しました");
		  case State::E_INVALID_TEMPO_BEAT:
		  	return (U"テンポまたは拍子が無効です");
		  case State::E_INVALID_LANE:
		    return (U"無効なレーンが存在します");
		  default:
		  	return (U"");
		}
	}

	

}
