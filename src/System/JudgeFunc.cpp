//
//  JudgeFunc.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/19.
//

#include "JudgeFunc.hpp"

#include <cmath>


namespace musicgame {

	namespace judgefunc {
	
		using judgefunc_return_t = TimingJudge::judgefunc_return_t;
		
	
		judgefunc_return_t begJudgeForEasy(
			const std::vector<const score::Note*> &notes,
			double inputTime
		) {
			
			const auto target = notes.front();

			JudgeState judge = JudgeState::NONE;
			
			if (target->type == score::NoteType::HIT) {
				// hit note
				
				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.075)
						judge = JudgeState::BEST;
					else if (error < 0.15)
						judge = JudgeState::BETTER;
					else if (error < 0.30)
						judge = JudgeState::GOOD;
				} else {
					if (error > -0.075)
						judge = JudgeState::BEST;
					else if (error > -0.10)
						judge = JudgeState::BETTER;
					else if (error > -0.20)
						judge = JudgeState::GOOD;
				}
				
				
			} else {
				// hold begin note

				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error < 0.15)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error < 0.30)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				} else {
					if (error > -0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error > -0.10)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error > -0.20)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				}

			}
			

			return judgefunc_return_t(judge, target->indexInLane);
		}

		judgefunc_return_t endJudgeForEasy(
			const score::Note* note, bool keyState, double inputTime
		) {
			JudgeState judge = JudgeState::NONE;
			
			if (keyState) {
				
				if (inputTime >= note->t_end.sec)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// user kept pressing key until the hold end note.
				else
					judge = JudgeState::NONE | JudgeState::HOLDCONTINUE;

			} else {

				if (inputTime - note->t_end.sec >= -0.075)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// when user input time is same / later to the note time
				else if (inputTime - note->t_end.sec > -0.15)
					judge = JudgeState::BETTER | JudgeState::HOLDFINISHED;	// when user input time is earlier to the note time
				else if (inputTime - note->t_end.sec > -0.40)
					judge = JudgeState::GOOD | JudgeState::HOLDFINISHED;	// same as above
				else
					judge = JudgeState::MISS | JudgeState::HOLDBREAK;	// same as above

			}
			
			
			return judgefunc_return_t(judge, note->indexInLane);
		}

		judgefunc_return_t missedJudgeForEasy(
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
		
		

		judgefunc_return_t begJudgeForNormal(
			const std::vector<const score::Note*> &notes,
			double inputTime
		) {
			
			const auto target = notes.front();

			JudgeState judge = JudgeState::NONE;
			
			if (target->type == score::NoteType::HIT) {
				// hit note
				
				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.06)
						judge = JudgeState::BEST;
					else if (error < 0.125)
						judge = JudgeState::BETTER;
					else if (error < 0.25)
						judge = JudgeState::GOOD;
				} else {
					if (error > -0.06)
						judge = JudgeState::BEST;
					else if (error > -0.10)
						judge = JudgeState::BETTER;
					else if (error > -0.20)
						judge = JudgeState::GOOD;
				}
				
				
			} else {
				// hold begin note

				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error < 0.15)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error < 0.30)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				} else {
					if (error > -0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error > -0.10)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error > -0.20)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				}

			}
			

			return judgefunc_return_t(judge, target->indexInLane);
		}

		judgefunc_return_t endJudgeForNormal(
			const score::Note* note, bool keyState, double inputTime
		) {
			JudgeState judge = JudgeState::NONE;
			
			if (keyState) {
				
				if (inputTime >= note->t_end.sec)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// user kept pressing key until the hold end note.
				else
					judge = JudgeState::NONE | JudgeState::HOLDCONTINUE;

			} else {

				if (inputTime - note->t_end.sec >= -0.075)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// when user input time is same / later to the note time
				else if (inputTime - note->t_end.sec > -0.15)
					judge = JudgeState::BETTER | JudgeState::HOLDFINISHED;	// when user input time is earlier to the note time
				else if (inputTime - note->t_end.sec > -0.40)
					judge = JudgeState::GOOD | JudgeState::HOLDFINISHED;	// same as above
				else
					judge = JudgeState::MISS | JudgeState::HOLDBREAK;	// same as above

			}
			
			
			return judgefunc_return_t(judge, note->indexInLane);
		}

		judgefunc_return_t missedJudgeForNormal(
			const score::Note* note,
			double inputTime
		) {
			
			JudgeState judge = JudgeState::NONE;
			
			switch (note->type) {
			case score::NoteType::HIT:
				
				if (inputTime - note->t_beg.sec >= 0.25)
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



		judgefunc_return_t begJudgeForHard(
			const std::vector<const score::Note*> &notes,
			double inputTime
		) {
			
			const auto target = notes.front();

			JudgeState judge = JudgeState::NONE;
			
			if (target->type == score::NoteType::HIT) {
				// hit note
				
				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.05)
						judge = JudgeState::BEST;
					else if (error < 0.10)
						judge = JudgeState::BETTER;
					else if (error < 0.20)
						judge = JudgeState::GOOD;
				} else {
					if (error > -0.05)
						judge = JudgeState::BEST;
					else if (error > -0.10)
						judge = JudgeState::BETTER;
					else if (error > -0.20)
						judge = JudgeState::GOOD;
				}
				
				
			} else {
				// hold begin note

				const double error = inputTime - target->t_beg.sec;
				
				if (error >= 0) {
					if (error < 0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error < 0.15)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error < 0.30)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				} else {
					if (error > -0.075)
						judge = JudgeState::BEST | JudgeState::HOLDCONTINUE;
					else if (error > -0.10)
						judge = JudgeState::BETTER | JudgeState::HOLDCONTINUE;
					else if (error > -0.20)
						judge = JudgeState::GOOD | JudgeState::HOLDCONTINUE;
				}

			}
			

			return judgefunc_return_t(judge, target->indexInLane);
		}

		judgefunc_return_t endJudgeForHard(
			const score::Note* note, bool keyState, double inputTime
		) {
			JudgeState judge = JudgeState::NONE;
			
			if (keyState) {
				
				if (inputTime >= note->t_end.sec)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// user kept pressing key until the hold end note.
				else
					judge = JudgeState::NONE | JudgeState::HOLDCONTINUE;

			} else {

				if (inputTime - note->t_end.sec >= -0.05)
					judge = JudgeState::BEST | JudgeState::HOLDFINISHED;	// when user input time is same / later to the note time
				else if (inputTime - note->t_end.sec > -0.15)
					judge = JudgeState::BETTER | JudgeState::HOLDFINISHED;	// when user input time is earlier to the note time
				else if (inputTime - note->t_end.sec > -0.30)
					judge = JudgeState::GOOD | JudgeState::HOLDFINISHED;	// same as above
				else
					judge = JudgeState::MISS | JudgeState::HOLDBREAK;	// same as above

			}
			
			
			return judgefunc_return_t(judge, note->indexInLane);
		}

		judgefunc_return_t missedJudgeForHard(
			const score::Note* note,
			double inputTime
		) {
			
			JudgeState judge = JudgeState::NONE;
			
			switch (note->type) {
			case score::NoteType::HIT:
				
				if (inputTime - note->t_beg.sec >= 0.20)
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




		
	}

}
