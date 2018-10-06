#include "TimingJudge.hpp"

#include <cmath>

namespace musicgame {

	TimingJudge::TimingJudge(size_t numofKeys) noexcept
		: controller(numofKeys) {}

	TimingJudge::TimingJudge(
		// arguments
		size_t numofKeys,
		const notes_t &_notes,
		const beg_judge_func_t &_begJudgeFunc,
		const end_judge_func_t &_endJudgeFunc,
		const missed_judge_func_t &_missedJudgeFunc,
		double _enumRangeSec
	) noexcept :
		controller(numofKeys)
	{
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


		// update member variable	
		enumRangeSec = _enumRangeSec;
		begJudgeFunc = _begJudgeFunc;
		endJudgeFunc = _endJudgeFunc;
		missedJudgeFunc = _missedJudgeFunc;
	
		controller.create(numofKeys);

		size_t holdCnt = 0;
		for (const auto &n : _notes) {
			notes.at(n.lane).push_back(n);
			if (n.type == score::NoteType::HOLD)
				holdCnt++;
		}
		
		initJudge();
		
		results.reserve(_notes.size() + holdCnt);
		

		return true;
	};

	void TimingJudge::clear() {
		initAll();
	}
	
	TimingJudge& TimingJudge::input(size_t keyNum, bool isPressed) noexcept {
		if (keyNum < controller.size())
			controller.key(keyNum).update(isPressed);
		
		return *this;
	}

	std::vector<const JudgeResult*> TimingJudge::judge(double inputSec) noexcept {
		std::vector<const JudgeResult*> results;
		
		for (size_t i = 0; i < controller.size(); i++) {
			auto keyJudge = judgeForKey(i, inputSec);

			for (auto &j : keyJudge)
				results.emplace_back(j);
		}
		
		return results;
	}

	std::vector<const JudgeResult*> TimingJudge::judgeForKey(size_t keyNum, double inputSec) noexcept {
		std::vector<const JudgeResult*> addition;
		const size_t numofResults = results.size();
		
		Key& key = controller.key(keyNum);

		// judge for missed notes
		if (!judgingNote.at(keyNum)) {
			for (auto it = enumBegNote.at(keyNum); it != notes.at(keyNum).cend(); it++) {
				const judgefunc_return_t ret = missedJudgeFunc(&(*it), inputSec);
				if (ret.judge == Judgement::NONE)
					break;
				
				results.emplace_back(
					*it, ret.judge, inputSec - it->t_beg.sec
				);
			
				// update note enumeration start
				enumBegNote.at(keyNum) = it + 1;
			}
		}
		


		// judge by key down / released
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
			
//			if (judgedNote->type == score::NoteType::HIT) {
//				if (ret.judge == Judgement::HOLDBREAK)
//					ret.judge = Judgement::MISS;
//			}
			
		}
		
		
		
		// process the judged note
		if (ret.judge != Judgement::NONE) {
		
			const notes_t::const_iterator judgedNote
				= notes.at(keyNum).cbegin() + ret.indexInLane;
			
			
			// judge overtaken notes
			for (auto it = enumBegNote.at(keyNum); it != judgedNote; it++) {
				results.emplace_back(*it, Judgement::MISS, inputSec - it->t_beg.sec);
				enumBegNote.at(keyNum) = it + 1;
			}
			
			
			// create user-input-judge result
			double error;
			if (judgingNote.at(keyNum)) {
				// hold end note
				if (ret.judge == Judgement::HOLDCONTINUE)
					error = 0;	// for hold middle judge.
				else
					error = inputSec - judgedNote->t_end.sec;
					
			} else {
				error = inputSec - judgedNote->t_beg.sec; // hit note or hold begin note
			}
			
			results.emplace_back(
				*judgedNote, ret.judge, error
			);
			
			
		
			if (judgingNote.at(keyNum)) {
				// hold note
			
				if (ret.judge != Judgement::HOLDCONTINUE) {
					// update judging note
					judgingNote.at(keyNum) = nullptr;  // end reference
					// update note enumeration start
					enumBegNote.at(keyNum) = judgedNote + 1;
				}
				
			} else {
			
				if (judgedNote->type == score::NoteType::HOLD) {
					// previous judged note is hold note
					judgingNote.at(keyNum) = &*judgedNote; // start reference
				} else {
					// update note enumeration start
					enumBegNote.at(keyNum) = judgedNote + 1;
				}
				
			}
		
		}

		
		createAdditionalResults(addition, numofResults);
		return addition;
	}

	const std::vector<JudgeResult> &TimingJudge::getResults() const noexcept {
		return results;
	}
	
	const score::Note* TimingJudge::getJudgeStartNote(int keyNum) const noexcept {
		if (keyNum < 0 || keyNum >= score::numofLanes)
			return nullptr;
		
		if (enumBegNote.at(keyNum) == notes.at(keyNum).cend())
			return nullptr;
		else
			return &*enumBegNote.at(keyNum);
	}
	
	const score::Note* TimingJudge::getJudgingHoldNote(int keyNum) const noexcept {
		if (keyNum < 0 || keyNum >= score::numofLanes)
			return nullptr;
		
		return judgingNote.at(keyNum);
	}
	
	
	
	void TimingJudge::restart() noexcept {
		size_t capacity = results.capacity();
		initJudge();
		results.reserve(capacity);
	}


	void TimingJudge::initAll() {
		results.clear();
		for (auto &n : notes)
			n.clear();
		for (auto &n : judgingNote)
			n = nullptr;
		enumRangeSec = 0.0;
		controller.clear();
	}
	
	void TimingJudge::initJudge() {
		for (auto &n : judgingNote)
			n = nullptr;
		results.clear();
		for (size_t i = 0; i < score::numofLanes; i++) {
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

		Judgement judge = Judgement::NONE;
		
		if (target->type == score::NoteType::HIT) {
			// hit note
			
			if (abs(inputTime - target->t_beg.sec) < 0.05)
				judge = Judgement::BEST;
			else if (abs(inputTime - target->t_beg.sec) < 0.10)
				judge = Judgement::BETTER;
			else if (abs(inputTime - target->t_beg.sec) < 0.30)
				judge = Judgement::GOOD;

		} else {
			// hold begin note

			if (abs(inputTime - target->t_beg.sec) < 0.20)
				judge = Judgement::BEST;

		}
		

		return judgefunc_return_t(judge, target->indexInLane);
	}

	TimingJudge::judgefunc_return_t TimingJudge::defaultEndJudgeFunc(
		const score::Note* note, bool keyState, double inputTime
	) {
		Judgement judge = Judgement::NONE;
		
		if (keyState) {
			
			if (inputTime >= note->t_end.sec)
				judge = Judgement::HOLDFINISHED;	// user kept pressing key until the hold end note.
			else
				//judge = Judgement::HOLDCONTINUE;
				judge = Judgement::NONE;

		} else {

			if (inputTime - note->t_end.sec >= 0.0)
				judge = Judgement::HOLDFINISHED;	// when user input time is same / later to the note time
			else if (inputTime - note->t_end.sec > -0.3)
				judge = Judgement::BETTER;	// when user input time is earlier to the note time
			else if (inputTime - note->t_end.sec > -0.6)
				judge = Judgement::BETTER;	// same as above
			else
				judge = Judgement::GOOD;	// same as above

		}
		
		
		return judgefunc_return_t(judge, note->indexInLane);
	}

	TimingJudge::judgefunc_return_t TimingJudge::defaultMissedJudgeFunc(
		const score::Note* note,
		double inputTime
	) {
		
		Judgement judge = Judgement::NONE;
		
		switch (note->type) {
		case score::NoteType::HIT:
			
			if (inputTime - note->t_beg.sec >= 0.30)
				judge = Judgement::MISS;
			
		    break;
		case score::NoteType::HOLD:
		
			if (inputTime - note->t_beg.sec >= 0.30)
				//judge = Judgement::HOLDBREAK;
				judge = Judgement::MISS;
			
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
