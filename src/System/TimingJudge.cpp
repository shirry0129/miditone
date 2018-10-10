#include "TimingJudge.hpp"

#include <cmath>

namespace musicgame {

	

	TimingJudge::TimingJudge() noexcept {}

	TimingJudge::TimingJudge(
		// arguments
		size_t numofKeys,
		const notes_t &_notes,
		const beg_judge_func_t &_begJudgeFunc,
		const end_judge_func_t &_endJudgeFunc,
		const missed_judge_func_t &_missedJudgeFunc,
		double _enumRangeSec
	) noexcept {
		create(numofKeys, _notes, _begJudgeFunc, _endJudgeFunc, _missedJudgeFunc, _enumRangeSec);
	}

	TimingJudge::~TimingJudge() {};

	bool TimingJudge::create(
		size_t numofKeys,
		const notes_t &_notes,
		const beg_judge_func_t &_begJudgeFunc,
		const end_judge_func_t &_endJudgeFunc,
		const missed_judge_func_t &_missedJudgeFunc,
		double _enumRangeSec
	) noexcept {
		if (_enumRangeSec <= 0.0)
			return false;

		initAll();
		
		notes.resize(4);
		controller.create(numofKeys);
		judgingNote.resize(numofKeys);
		enumBegNote.resize(numofKeys);

		// update member variable
		size_t holdCnt = 0;
		for (const auto &n : _notes) {
			notes.at(n.lane).push_back(n);
			if (n.type == score::NoteType::HOLD)
				holdCnt++;
		}
		
		enumRangeSec = _enumRangeSec;
		begJudgeFunc = _begJudgeFunc;
		endJudgeFunc = _endJudgeFunc;
		missedJudgeFunc = _missedJudgeFunc;
	
		readyJudge();
		
		results.reserve(_notes.size() + holdCnt);
		

		return true;
	};

	void TimingJudge::clear() {
		initAll();
	}
	
	TimingJudge& TimingJudge::input(size_t keyNum, bool isPressed) noexcept {
		if (keyNum < controller.numofKeys())
			controller.key(keyNum).update(isPressed);
		
		return *this;
	}
	
	TimingJudge& TimingJudge::inputAuto(double currentSec) noexcept {
		for (size_t i = 0; i < score::numofLanes; i++) {
			if (judgingNote.at(i)) {
				// when judge the hold note
				if (currentSec < judgingNote.at(i)->t_end.sec) {
					// before the hold end note
					controller.key(i).update(true);
				} else {
					controller.key(i).update(false);
				}
			} else {
				// when judge the hit note / the hold begin note
				if (currentSec >= enumBegNote.at(i)->t_beg.sec)
					controller.key(i).update(true);
				else
					controller.key(i).update(false);
			}
		}
		
		return *this;
	}
	
	std::vector<const JudgeResult*> TimingJudge::judge(double inputSec) noexcept {
		std::vector<const JudgeResult*> results;
		
		for (size_t i = 0; i < controller.numofKeys(); i++) {
			auto keyJudge = judgeForKey(i, inputSec);

			for (auto &j : keyJudge)
				results.emplace_back(j);
		}
		
		return results;
	}
	
	
	// judge by following order.
	// 1. judge missed notes (except in the middle of judgeing the hold end note).
	// 2. judge by user input.
	// 3. judge overtaken note.
	std::vector<const JudgeResult*> TimingJudge::judgeForKey(size_t keyNum, double inputSec) noexcept {
		std::vector<const JudgeResult*> addition;
		const size_t numofResults = results.size();
		
		Key& key = controller.key(keyNum);

		//
		// 1. judge for missed notes
		//
		if (!judgingNote.at(keyNum)) {
			for (auto it = enumBegNote.at(keyNum); it != notes.at(keyNum).cend(); it++) {
				const judgefunc_return_t ret = missedJudgeFunc(&(*it), inputSec);
				if (!ret.judge.isJudged())
					break;
				
				results.emplace_back(
					*it, ret.judge, inputSec - it->t_beg.sec
				);
				
				if (it->type == score::NoteType::HOLD && ret.judge.isContinueHold()) {
					// start reference
					judgingNote.at(keyNum) = &*it;
				} else {
					// update note enumeration start
					enumBegNote.at(keyNum) = it + 1;
				}
		
			}
		}
		


		//
		// 2. judge by user input
		//
		judgefunc_return_t ret;
		
		if (judgingNote.at(keyNum) != nullptr) {
			// are judging the hold note
			// judge
			ret = endJudgeFunc(judgingNote.at(keyNum), key.isOn(), inputSec);
		} else {
			// are NOT judging the hold note
			if (!key.isPressedMoment()) {
				createAdditionalResults(addition, numofResults);
				return addition;
			}
			
			// enumerate the notes for judge
			std::vector<const score::Note*> enumNotes;
			enumJudgeNotes(
				enumNotes, enumBegNote.at(keyNum), notes.at(keyNum).cend(),
				inputSec, enumRangeSec
			);
			
			if (enumNotes.empty()) {
				createAdditionalResults(addition, numofResults);
				return addition;
			}
			
			// judge
			ret = begJudgeFunc(enumNotes, inputSec);
		}
		
		if (ret.judge.getJudge() == JudgeState::NONE) {
			createAdditionalResults(addition, numofResults);
			return addition;
		}
		
		
		
		//
		// process the judged note
		//
		const notes_t::const_iterator judgedNote
			= notes.at(keyNum).cbegin() + ret.indexInLane;
		
		
		// 3. judge overtaken notes
		for (auto it = enumBegNote.at(keyNum); it != judgedNote; it++) {
			results.emplace_back(*it, JudgeState::MISS, inputSec - it->t_beg.sec);
			enumBegNote.at(keyNum) = it + 1;
		}
		
		// calculation error
		double error;
		if (judgingNote.at(keyNum)) {
			// hold end note
			if (ret.judge.isContinueHold())
				error = 0;	// for hold middle judge.
			else
				error = inputSec - judgedNote->t_end.sec;
				
		} else {
			error = inputSec - judgedNote->t_beg.sec; // hit note or hold begin note
		}
		
		// store result
		results.emplace_back(
			*judgedNote, ret.judge, error
		);
		
		
		if (judgingNote.at(keyNum)) {
			// hold note
			if (!ret.judge.isContinueHold()) {
				// update judging note
				judgingNote.at(keyNum) = nullptr;  // end reference
				// update note enumeration start
				enumBegNote.at(keyNum) = judgedNote + 1;
			}
		} else {
			if (judgedNote->type == score::NoteType::HOLD &&
				ret.judge.isContinueHold()) {
				// previous judged note is hold note
				judgingNote.at(keyNum) = &*judgedNote; // start reference
			} else {
				// update note enumeration start
				enumBegNote.at(keyNum) = judgedNote + 1;
			}
		}
		
		createAdditionalResults(addition, numofResults);
		return addition;
	}

	const std::vector<JudgeResult> &TimingJudge::getResults() const noexcept {
		return results;
	}
	
	const score::Note* TimingJudge::getJudgeStartNote(int keyNum) const noexcept {
		if (keyNum < 0 || keyNum >= controller.numofKeys())
			return nullptr;
		
		if (enumBegNote.at(keyNum) == notes.at(keyNum).cend())
			return nullptr;
		else
			return &*enumBegNote.at(keyNum);
	}
	
	const score::Note* TimingJudge::getJudgingHoldNote(int keyNum) const noexcept {
		if (keyNum < 0 || keyNum >= controller.numofKeys())
			return nullptr;
		
		return judgingNote.at(keyNum);
	}
	
	
	
	void TimingJudge::restart() noexcept {
		size_t capacity = results.capacity();
		readyJudge();
		results.reserve(capacity);
	}


	void TimingJudge::initAll() {
		results.clear();
		notes.clear();
		judgingNote.clear();
		enumRangeSec = 0.0;
		controller.clear();
	}
	
	void TimingJudge::readyJudge() {
		for (auto &n : judgingNote)
			n = nullptr;
		
		results.clear();
	
		for (size_t i = 0; i < controller.numofKeys(); i++) {
			if (!notes.at(i).empty())
				enumBegNote.at(i) = notes.at(i).begin();
			else
				enumBegNote.at(i) = notes.at(i).end();
		}
	}
	
	void TimingJudge::createAdditionalResults(
		std::vector<const JudgeResult *> &v, size_t begIndex
	) {
		v.clear();
		for (size_t i = begIndex; i < results.size(); i++) {
			v.push_back(&results.at(i));
		}
	}


	TimingJudge::judgefunc_return_t TimingJudge::defaultBegJudgeFunc(
		const std::vector<const score::Note*> &notes,
		double inputTime
	) {
		
		const auto target = notes.front();

		JudgeState judge = JudgeState::NONE;
		
		if (target->type == score::NoteType::HIT) {
			// hit note
			
			if (abs(inputTime - target->t_beg.sec) < 0.05)
				judge = JudgeState::BEST;
			else if (abs(inputTime - target->t_beg.sec) < 0.10)
				judge = JudgeState::BETTER;
			else if (abs(inputTime - target->t_beg.sec) < 0.30)
				judge = JudgeState::GOOD;

		} else {
			// hold begin note

			if (abs(inputTime - target->t_beg.sec) < 0.20)
				judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;

		}
		

		return judgefunc_return_t(judge, target->indexInLane);
	}

	TimingJudge::judgefunc_return_t TimingJudge::defaultEndJudgeFunc(
		const score::Note* note, bool keyState, double inputTime
	) {
		JudgeState judge = JudgeState::NONE;
		
		if (keyState) {
			
			if (inputTime >= note->t_end.sec)
				judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// user kept pressing key until the hold end note.
			else
				//judge = Judgement::HOLDCONTINUE;
				judge = JudgeState::NONE | JudgeState::HOLDCONTINUE;

		} else {

			if (inputTime - note->t_end.sec >= 0.0)
				judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// when user input time is same / later to the note time
			else if (inputTime - note->t_end.sec > -0.2)
				judge = JudgeState::BETTER | JudgeState::HOLDFINISHED;	// when user input time is earlier to the note time
			else if (inputTime - note->t_end.sec > -0.4)
				judge = JudgeState::GOOD | JudgeState::HOLDFINISHED;	// same as above
			else
				judge = JudgeState::NOTBAD | JudgeState::HOLDFINISHED;	// same as above

		}
		
		
		return judgefunc_return_t(judge, note->indexInLane);
	}

	TimingJudge::judgefunc_return_t TimingJudge::defaultMissedJudgeFunc(
		const score::Note* note,
		double inputTime
	) {
		
		JudgeState judge = JudgeState::NONE;
		
		switch (note->type) {
		case score::NoteType::HIT:
			
			if (inputTime - note->t_beg.sec >= 0.30)
				judge = JudgeState::MISS;
			
		    break;
		case score::NoteType::HOLD:
		
			if (inputTime - note->t_beg.sec >= 0.30)
				//judge = Judgement::HOLDBREAK;
				judge = JudgeState::MISS | JudgeState::HOLDBREAK;
			
	  		break;
		}
		
	
		return judgefunc_return_t(judge, note->indexInLane);
	}
	
	void TimingJudge::enumJudgeNotes(
		std::vector<const score::Note *> &notes,
		notes_t::const_iterator begin,
		notes_t::const_iterator end,
		double inputSec,
		double rangeSec
	) {
	
		for (auto it = begin; it != end; it++) {
			if (it->t_beg.sec < inputSec - enumRangeSec)
				continue;	// user input time is much earlier
			if (it->t_beg.sec > inputSec + enumRangeSec)
				break;		// user input time is much later

			notes.push_back(&(*it));
		}
		
	}
	
	

}
