
/*
This class compares the time of user key input and the note time,
then return that result.

Author	: Nanami Yamamoto
Date	: 2018/9/26
*/


#ifndef _TIMINGJUDGE_HPP_
#define _TIMINGJUDGE_HPP_


#include <functional>
#include <array>

#include "ScoreManager.hpp"


namespace musicgame {

	enum class Judgement {
		BEST = 0,
		BETTER,
		GOOD,
		NOTBAD,
		BAD,
		MISS,
		NONE
		};

	struct JudgeResult : score::Note {
		Judgement result;
		double error;
	};

	enum struct JudgeState {
		MIDDLE = 0,
		END
	};


	class TimingJudge {
	public:
		using notes_t = std::vector<score::Note>;
		
		// for hit notes / hold begin notes
		using judge_beg_func_t = std::function<
			Judgement(notes_t::const_iterator beg, notes_t::const_iterator end, double inputTime)
		>;

		// for hold end notes
		using judge_end_func_t = std::function<
			Judgement(notes_t::const_iterator note, JudgeState state, double inputTime)
		>;

	
		TimingJudge(
			const notes_t &_notes,
			const judge_beg_func_t &_judgeBegFunc = defaultJudgeBegFunc,
			const judge_end_func_t &_judgeEndFunc = defaultJudgeEndFunc,
			double enumRangeSec = 1.0 // amplitude
		) noexcept;

		~TimingJudge();


		bool create(
			const notes_t &_notes,
			const judge_beg_func_t &_judgeBegFunc = defaultJudgeBegFunc,
			const judge_end_func_t &_judgeEndFunc = defaultJudgeEndFunc,
			double enumRangeSec = 1.0 // amplitude
		) noexcept;

		void clear();

		const JudgeResult& judge(double inputSec, bool keyState, int keyNum) noexcept;


	private:
		std::array<notes_t, score::numofLanes> notes;

		judge_beg_func_t judgeBegFunc;
		judge_end_func_t judgeEndFunc;

		bool isReady;

		std::vector<JudgeResult> result;

		std::array<score::Note*, 4> judgingNote;

		void init();

		static Judgement defaultJudgeBegFunc(
			notes_t::const_iterator beg, notes_t::const_iterator end, double inputTime
		);

		static Judgement defaultJudgeEndFunc(
			notes_t::const_iterator note, JudgeState state, double inputTime
		);

		

	};


}


#endif // !_TIMINGJUDGE_HPP_
