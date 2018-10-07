
/*
This class compares the time of user key input and the note time,
then return that result.

Author	: Nanami Yamamoto
Date	: 2018/9/26
*/


#ifndef _TIMINGJUDGE_HPP_
#define _TIMINGJUDGE_HPP_


#include <functional>

#include "ScoreManager.hpp"
#include "Controller.hpp"
#include "Judgement.hpp"


namespace musicgame {


	

	struct JudgeResult : score::Note {
		JudgeResult(const Note &_note, const Judgement &_result, double _error) noexcept
			: Note(_note), result(_result), error(_error) {}

		const Judgement result;
		// negative value express that user input is earlier than exact time
		const double error;
	};
	
	using judge_results_t = std::vector<const JudgeResult*>;
	
	
	
	

	class TimingJudge {
	public:
		using notes_t = std::vector<score::Note>;
		
        struct judgefunc_return_t {
			judgefunc_return_t() {}
			judgefunc_return_t(const Judgement &_judge, int _indexInLane)
        		: judge(_judge), indexInLane(_indexInLane) {}
			
			Judgement judge;
			int indexInLane;
        };
		
		// for hit notes / hold begin notes
		using beg_judge_func_t = std::function<
			judgefunc_return_t(
				const std::vector<const score::Note*> &notes,
				double inputTime
			)
		>;

		// for hold end notes
		using end_judge_func_t = std::function<
			judgefunc_return_t(
				const score::Note* note,
				bool keyState,
				double inputTime
			)
		>;
		
		// for missed judge
		using missed_judge_func_t = std::function<
			judgefunc_return_t(
				const score::Note* note,
				double inputTime
			)
		>;



		TimingJudge() noexcept;
	
		TimingJudge(
			size_t numofKeys,
			const notes_t &_notes,
			const beg_judge_func_t &_judgeBegFunc = defaultBegJudgeFunc,
			const end_judge_func_t &_judgeEndFunc = defaultEndJudgeFunc,
			const missed_judge_func_t &_judgeMissedFunc = defaultMissedJudgeFunc,
			double _enumRangeSec = 1.0 // amplitude
		) noexcept;

		~TimingJudge();

		bool create(
			size_t numofKeys,
			const notes_t &_notes,
			const beg_judge_func_t &_judgeBegFunc = defaultBegJudgeFunc,
			const end_judge_func_t &_judgeEndFunc = defaultEndJudgeFunc,
			const missed_judge_func_t &_judgeMissedFunc = defaultMissedJudgeFunc,
			double _enumRangeSec = 1.0 // amplitude
		) noexcept;

		void clear();
		
		TimingJudge& input(
			size_t keyNum,
			bool isPressed
		) noexcept;

		judge_results_t judge(
			double inputSec
		) noexcept;
		
		const std::vector<JudgeResult> &getResults() const noexcept;
		
		const score::Note* getJudgeStartNote(int keyNum) const noexcept;
		
		const score::Note* getJudgingHoldNote(int keyNum) const noexcept;
		
		void restart() noexcept;

	
	private:
	
		Controller controller;
		
		std::vector<notes_t> notes;
	
		beg_judge_func_t begJudgeFunc;
		end_judge_func_t endJudgeFunc;
		missed_judge_func_t missedJudgeFunc;
	
		double enumRangeSec;

		std::vector<notes_t::const_iterator> enumBegNote;
	
		std::vector<const score::Note*> judgingNote;
		
		std::vector<JudgeResult> results;
		
		
		void initAll();
		void readyJudge();
		
		std::vector<const JudgeResult*> judgeForKey(
			size_t keyNum,
			double inputSec
		) noexcept;
		
		void enumJudgeNotes(
			std::vector<const score::Note*> &notes,
			notes_t::const_iterator begin,
			notes_t::const_iterator end,
			double inputSec,
			double rangeSec
		);
		
		void createAdditionalResults(
			std::vector<const JudgeResult*> &v,
			size_t begIndex
		);

		static judgefunc_return_t defaultBegJudgeFunc(
			const std::vector<const score::Note*> &notes,
			double inputTime
		);

		static judgefunc_return_t defaultEndJudgeFunc(
			const score::Note* note, bool keyState, double inputTime
		);
		
		static judgefunc_return_t defaultMissedJudgeFunc(
			const score::Note* note,
			double inputTime
		);

		

	};


}


#endif // !_TIMINGJUDGE_HPP_
