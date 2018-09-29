#include "TimingJudge.hpp"

#include <cmath>

namespace musicgame {

	TimingJudge::TimingJudge(
		// arguments
		const notes_t &_notes,
		const judge_beg_func_t &_judgeBegFunc,
		const judge_end_func_t &_judgeEndFunc,
		double _enumRangeSec
	) noexcept : 
		// initializetion for constructor
		isReady(false), 
		dummyResult(
			JudgeResult(
				score::Note(score::NoteType::HIT, 0, 0, score::NoteTime(0, 0.0), score::NoteTime(0, 0.0)), 
				Judgement::NONE, 
				0.0
			)
		)
	{
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

		isReady = true;


		return true;
	};

	void TimingJudge::clear() {
		init();
	}

	const JudgeResult & TimingJudge::judge(double inputSec, bool keyState, int keyNum) noexcept {
		if (keyNum < 0 || keyNum > score::numofLanes)
			 return dummyResult;


		if (judgingNote.at(keyNum) != nullptr) {
			// are judging the hold note

			JudgeState state = (keyState) ? JudgeState::MIDDLE : JudgeState::END;

			judgeEndFunc(judgingNote.at(keyNum), state, inputSec);

		} else {
			// are NOT judging the hold note


			// enumerate the notes for judge
			std::vector<const score::Note*> enumNotes;

			for (auto it = enumBegNote.at(keyNum); it != notes.at(keyNum).end(); it++) {
				if (inputSec < it->t_beg.sec - enumRangeSec)
					continue;	// user input time is much earlier
				if (inputSec > it->t_beg.sec + enumRangeSec)
					break;		// user input time is much later

				enumNotes.push_back(&(*it));
			}


			// judge
			judgeBegFunc(enumNotes, inputSec);

		}


		return result.back();
	}



	void TimingJudge::init() {
		isReady = false;
		result.clear();
		for (auto &n : notes)
			n.clear();
		for (auto &n : judgingNote)
			n = nullptr;
		enumRangeSec = 0.0;
	}


	Judgement TimingJudge::defaultJudgeBegFunc(const std::vector<const score::Note*> &notes, double inputTime) {
		
		const auto target = notes.front();

		if (target->type == score::NoteType::HIT) {
			// hit note


			if (abs(inputTime - target->t_beg.sec) < 0.10)
				return Judgement::BEST;
			if (abs(inputTime - target->t_beg.sec) < 0.20)
				return Judgement::BETTER;
			if (abs(inputTime - target->t_beg.sec) < 0.40)
				return Judgement::GOOD;
			if (abs(inputTime - target->t_beg.sec) < 0.50)
				return Judgement::MISS;

		} else {
			// hold (begin) note 

			if (abs(inputTime - target->t_beg.sec) < 0.20)
				return Judgement::BEST;

		}
		

		return Judgement::NONE;
	}

	Judgement TimingJudge::defaultJudgeEndFunc(const score::Note* note, JudgeState state, double inputTime) {
	
		if (state == JudgeState::MIDDLE) {

			// return Judgement::BEST; // CHUNITHM style
			return Judgement::NONE;

		} else {

			if (inputTime - note->t_end.sec >= 0.0)
				return Judgement::BEST;		// when user input time is same / later to the note time
			if (inputTime - note->t_end.sec > -0.3)
				return Judgement::BETTER;	// when user input time is earlier to the note time
			if (inputTime - note->t_end.sec > -0.6)
				return Judgement::BETTER;	// same as above
			else
				return Judgement::GOOD;		// same as above

		}

	}

}
