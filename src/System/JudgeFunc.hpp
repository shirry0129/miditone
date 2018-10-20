//
//  JudgeFunc.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/19.
//

#ifndef JudgeFunc_hpp
#define JudgeFunc_hpp


#include "TimingJudge.hpp"


/*
the negative value is earlier then exact time

#easy

>hit_begin
best ... +0.075s~ -0.075s
better.. +0.15s ~ -0.10s
good ... +0.30s ~ -0.20s
miss ...        ~ +0.30s

>hold_beg
same as hit_begin

>hold_end
best ... ~ -0.075s
better.. -0.075s ~ -0.15s
good ... -0.15s ~ -0.40s
miss ... -0.40s ~


#normal

>hit_begin
best ... +0.06s~ -0.06s
better.. +0.125s ~ -0.10s
good ... +0.25s ~ -0.20s
miss ...        ~ +0.25s

>hold_beg
best ... +0.075s~ -0.075s
better.. +0.15s ~ -0.10s
good ... +0.30s ~ -0.20s
miss ...        ~ +0.30s

>hold_end
best ... ~ -0.075s
better.. -0.075s ~ -0.15s
good ... -0.15s ~ -0.40s
miss ... -0.40s ~


#hard

>hit_begin
best ... +0.05s~ -0.05s
better.. +0.10s ~ -0.10s
good ... +0.20s ~ -0.20s
miss ...        ~ +0.20s

>hold_beg
best ... +0.075s~ -0.075s
better.. +0.15s ~ -0.10s
good ... +0.30s ~ -0.20s
miss ...        ~ +0.30s

>hold_end
best ... ~ -0.05s
better.. -0.05s ~ -0.15s
good ... -0.15s ~ -0.30s
miss ... -0.30s ~



*/


namespace musicgame {

	namespace judgefunc {

		TimingJudge::judgefunc_return_t begJudgeForEasy(
			const std::vector<const score::Note*> &notes,
			double inputTime
		);

		TimingJudge::judgefunc_return_t endJudgeForEasy(
			const score::Note* note, bool keyState, double inputTime
		);
	
		TimingJudge::judgefunc_return_t missedJudgeForEasy(
			const score::Note* note,
			double inputTime
		);
		
		
		TimingJudge::judgefunc_return_t begJudgeForNormal(
			const std::vector<const score::Note*> &notes,
			double inputTime
		);

		TimingJudge::judgefunc_return_t endJudgeForNormal(
			const score::Note* note, bool keyState, double inputTime
		);
	
		TimingJudge::judgefunc_return_t missedJudgeForNormal(
			const score::Note* note,
			double inputTime
		);

		
		TimingJudge::judgefunc_return_t begJudgeForHard(
			const std::vector<const score::Note*> &notes,
			double inputTime
		);

		TimingJudge::judgefunc_return_t endJudgeForHard(
			const score::Note* note, bool keyState, double inputTime
		);
	
		TimingJudge::judgefunc_return_t missedJudgeForHard(
			const score::Note* note,
			double inputTime
		);
		
	}
}


#endif /* JudgeFunc_hpp */
