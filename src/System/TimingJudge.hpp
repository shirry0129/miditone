
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
		JudgeResult(Note _note, Judgement _result, double _error) noexcept
			: Note(_note), result(_result), error(_error) {}

		const Judgement result;
		const double error;
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
			Judgement(const std::vector<const score::Note*> &notes, double inputTime)
		>;

		// for hold end notes
		using judge_end_func_t = std::function<
			Judgement(const score::Note* note, JudgeState state, double inputTime)
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
		std::array<notes_t::const_iterator, score::numofLanes> enumBegNote;

		judge_beg_func_t judgeBegFunc;
		judge_end_func_t judgeEndFunc;

		bool isReady;
		double enumRangeSec;

		std::vector<JudgeResult> result;

		std::array<score::Note*, 4> judgingNote;

		JudgeResult dummyResult;

		void init();

		static Judgement defaultJudgeBegFunc(
			const std::vector<const score::Note*> &notes, double inputTime
		);

		static Judgement defaultJudgeEndFunc(
			const score::Note* note, JudgeState state, double inputTime
		);

		

	};


}


#endif // !_TIMINGJUDGE_HPP_
