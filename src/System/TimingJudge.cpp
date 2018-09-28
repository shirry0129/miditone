#include "TimingJudge.hpp"

#include <cmath>

namespace musicgame {

	TimingJudge::TimingJudge(
		const notes_t &_notes,
		const judge_beg_func_t &_judgeBegFunc,
		const judge_end_func_t &_judgeEndFunc,
		double enumRangeSec
	) noexcept : isReady(false) {
		create(_notes, _judgeBegFunc, _judgeEndFunc, enumRangeSec);
	}

	TimingJudge::~TimingJudge() {};

	bool TimingJudge::create(
		const notes_t &_notes,
		const judge_beg_func_t &_judgeBegFunc,
		const judge_end_func_t &_judgeEndFunc,
		double enumRangeSec
	) noexcept {
		if (enumRangeSec <= 0.0)
			return false;

		init();


		// update member variable	
		judgeBegFunc = _judgeBegFunc;
		judgeEndFunc = _judgeEndFunc;

		for (const auto &n : _notes) {
			notes.at(n.lane).push_back(n);
		}

		isReady = true;


		return true;
	};

	void TimingJudge::clear() {
		init();
	}

	const JudgeResult & TimingJudge::judge(double inputSec, bool keyState, int keyNum) noexcept {
		

		return result.back();
	}



	void TimingJudge::init() {
		isReady = false;
		result.clear();
		for (auto &n : notes)
			n.clear();
		for (auto &n : judgingNote)
			n = nullptr;

	}


	Judgement TimingJudge::defaultJudgeBegFunc(notes_t::const_iterator beg, notes_t::const_iterator end, double inputTime) {
		
		if (beg->type == score::NoteType::HIT) {
			// hit note


			if (abs(inputTime - beg->t_beg.sec) < 0.10)
				return Judgement::BEST;
			if (abs(inputTime - beg->t_beg.sec) < 0.20)
				return Judgement::BETTER;
			if (abs(inputTime - beg->t_beg.sec) < 0.40)
				return Judgement::GOOD;
			if (abs(inputTime - beg->t_beg.sec) < 0.50)
				return Judgement::MISS;

		} else {
			// hold (begin) note 

			if (abs(inputTime - beg->t_beg.sec) < 0.20)
				return Judgement::BEST;

		}
		

		return Judgement::NONE;
	}

	Judgement TimingJudge::defaultJudgeEndFunc(notes_t::const_iterator note, JudgeState state, double inputTime) {
	
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
