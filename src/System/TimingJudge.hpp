
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
		NONE = 0,
		BEST,
		BETTER,
		GOOD,
		NOTBAD,
		BAD,
		MISS
		};

	struct JudgeResult : score::Note {
		JudgeResult(Note _note, Judgement _result, double _error) noexcept
			: Note(_note), result(_result), error(_error) {}

		const Judgement result;
		// negative value express that user input is earlier than exact time
		const double error;
	};

	enum struct JudgeState {
		MIDDLE = 0,
		END
	};


	class TimingJudge {
	public:
		using notes_t = std::vector<score::Note>;
		
        struct judgefunc_return_t {
        	judgefunc_return_t() {}
        	judgefunc_return_t(Judgement _judge, int _index)
        		: judge(_judge), index(_index) {}
			
            Judgement judge;
            int index;
        };
		
		// for hit notes / hold begin notes
		using judge_beg_func_t = std::function<
			judgefunc_return_t(const std::vector<const score::Note*> &notes, double inputTime)
		>;

		// for hold end notes
		using judge_end_func_t = std::function<
			judgefunc_return_t(const score::Note* note, JudgeState state, double inputTime)
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

		std::vector<const JudgeResult*> judge(double inputSec, bool keyState, int keyNum) noexcept;

	
	private:
		std::array<notes_t, score::numofLanes> notes;
		std::array<notes_t::const_iterator, score::numofLanes> enumBegNote;

		judge_beg_func_t judgeBegFunc;
		judge_end_func_t judgeEndFunc;
	
		double enumRangeSec;

		std::array<const score::Note*, 4> judgingNote;
		
		std::vector<JudgeResult> results;

		void init();
		void enumJudgeNotes(
			std::vector<const score::Note*> &notes,
			notes_t::const_iterator begin,
			notes_t::const_iterator end,
			double inputSec,
			double rangeSec
		);

		static judgefunc_return_t defaultJudgeBegFunc(
			const std::vector<const score::Note*> &notes, double inputTime
		);

		static judgefunc_return_t defaultJudgeEndFunc(
			const score::Note* note, JudgeState state, double inputTime
		);

		

	};


}


#endif // !_TIMINGJUDGE_HPP_
