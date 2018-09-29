#include "TimingJudge.hpp"

#include <cmath>

namespace musicgame {

	TimingJudge::TimingJudge(
		// arguments
		const notes_t &_notes,
		const judge_beg_func_t &_judgeBegFunc,
		const judge_end_func_t &_judgeEndFunc,
		double _enumRangeSec
	) noexcept {
		create(_notes, _judgeBegFunc, _judgeEndFunc, _enumRangeSec);
	}

	TimingJudge::~TimingJudge() {};

	bool TimingJudge::create(
		const notes_t &_notes,
		const judge_beg_func_t &_judgeBegFunc,
		const judge_end_func_t &_judgeEndFunc,
		double _enumRangeSec
	) noexcept {
		if (_enumRangeSec <= 0.0)
			return false;

		init();


		// update member variable	
		enumRangeSec = _enumRangeSec;
		judgeBegFunc = _judgeBegFunc;
		judgeEndFunc = _judgeEndFunc;

		for (const auto &n : _notes)
			notes.at(n.lane).push_back(n);

		for (size_t i = 0; i < score::numofLanes; i++) {
			if (!notes.at(i).empty())
				enumBegNote.at(i) = notes.at(i).begin();
			else
				enumBegNote.at(i) = notes.at(i).end();
		}

		return true;
	};

	void TimingJudge::clear() {
		init();
	}

	std::vector<const JudgeResult*> TimingJudge::judge(double inputSec, bool keyState, int keyNum) noexcept {
		std::vector<const JudgeResult*> presentResults;
	
		if (keyNum < 0 || keyNum > score::numofLanes)
			 return presentResults;


		judgefunc_return_t ret;
		
		if (judgingNote.at(keyNum) != nullptr) {
			// are judging the hold note

			// set current state.
			// note: key is down -> continue judging / key is released -> end judging
			const JudgeState state
				= (keyState) ? JudgeState::MIDDLE : JudgeState::END;
			
			// judge
			ret = judgeEndFunc(judgingNote.at(keyNum), state, inputSec);
			
		} else {
			// are NOT judging the hold note
			
			if (!keyState)
				return presentResults;
			
			// enumerate the notes for judge
			std::vector<const score::Note*> enumNotes;
			enumJudgeNotes(
				enumNotes, enumBegNote.at(keyNum), notes.at(keyNum).cend(),
				inputSec, enumRangeSec
			);
			
			// judge
			ret = judgeBegFunc(enumNotes, inputSec);
			
			const auto &judgedNote = notes.at(keyNum).at(ret.index);
			
			if (judgedNote.type == score::NoteType::HOLD) {
				// previous judged note is hold note
				judgingNote.at(keyNum) = &judgedNote; // start reference
			}
		}
		
		
		if (ret.judge != Judgement::NONE) {
			const notes_t::const_iterator judgedNoteItr
				= notes.at(keyNum).begin() + ret.index;
			
			
			// judge for missed notes
			auto it = notes.at(keyNum).begin() + enumBegNote.at(keyNum)->index;
			for (; it != judgedNoteItr; it++) {
				results.emplace_back(
					*it, Judgement::MISS, inputSec - it->t_beg.sec
				);
				
				presentResults.push_back(&results.back());
			}
			
		
			// create judge result
			double error;
			if (judgingNote.at(keyNum))
				error = inputSec - judgedNoteItr->t_end.sec; // hold end note
			else
				error = inputSec - judgedNoteItr->t_beg.sec; // hit note or hold begin note
			
			results.emplace_back(
				*judgedNoteItr, ret.judge, error
			);
			presentResults.push_back(&results.back());
			
			
			// update judging note
			if (judgingNote.at(keyNum))
				judgingNote.at(keyNum) = nullptr;  // end reference
		
			// update note enumeration start
			enumBegNote.at(keyNum) = judgedNoteItr + 1;
		}

		
		
		return presentResults;
	}



	void TimingJudge::init() {
		results.clear();
		for (auto &n : notes)
			n.clear();
		for (auto &n : judgingNote)
			n = nullptr;
		enumRangeSec = 0.0;
	}


	TimingJudge::judgefunc_return_t TimingJudge::defaultJudgeBegFunc(
		const std::vector<const score::Note*> &notes,
		double inputTime
	) {
		
		const auto target = notes.front();

		Judgement judge = Judgement::NONE;
		
		if (target->type == score::NoteType::HIT) {
			// hit note
			
			if (abs(inputTime - target->t_beg.sec) < 0.10)
				judge = Judgement::BEST;
			if (abs(inputTime - target->t_beg.sec) < 0.20)
				judge =  Judgement::BETTER;
			if (abs(inputTime - target->t_beg.sec) < 0.40)
				judge = Judgement::GOOD;
			if (abs(inputTime - target->t_beg.sec) < 0.50)
				judge = Judgement::MISS;

		} else {
			// hold (begin) note 

			if (abs(inputTime - target->t_beg.sec) < 0.20)
				judge = Judgement::BEST;

		}
		

		return judgefunc_return_t(judge, target->index);
	}

	TimingJudge::judgefunc_return_t TimingJudge::defaultJudgeEndFunc(
		const score::Note* note, JudgeState state, double inputTime
	) {
		Judgement judge;
		
		if (state == JudgeState::MIDDLE) {
			
			// return Judgement::BEST; // CHUNITHM style
			judge = Judgement::NONE;

		} else {

			if (inputTime - note->t_end.sec >= 0.0)
				judge = Judgement::BEST;		// when user input time is same / later to the note time
			if (inputTime - note->t_end.sec > -0.3)
				judge = Judgement::BETTER;	// when user input time is earlier to the note time
			if (inputTime - note->t_end.sec > -0.6)
				judge = Judgement::BETTER;	// same as above
			else
				judge = Judgement::GOOD;	// same as above

		}
		
		
		return judgefunc_return_t(judge, note->index);
	}
	
	void TimingJudge::enumJudgeNotes(
		std::vector<const score::Note *> &notes,
		notes_t::const_iterator begin,
		notes_t::const_iterator end,
		double inputSec,
		double rangeSec
	) {
	
		for (auto it = begin; it != end; it++) {
				if (inputSec < it->t_beg.sec - enumRangeSec)
					continue;	// user input time is much earlier
				if (inputSec > it->t_beg.sec + enumRangeSec)
					break;		// user input time is much later

				notes.push_back(&(*it));
		}
		
	}
	
	

}
