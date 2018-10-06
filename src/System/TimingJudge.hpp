
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
#include "Controller.hpp"


namespace musicgame {

	enum class Judgement {
		// don't judge, so don't store that result.
		NONE			= 0x00,
		
		// judge, and store that result.
		BEST			= 0x01,
		
		// same as above
		BETTER			= 0x02,
		
		// same as above
		GOOD			= 0x03,
		
		// same as above
		NOTBAD			= 0x04,
		
		// same as above
		BAD				= 0x05,
		
		// same as above
		MISS			= 0x06,
		
		
		//
		// following constants has extra meaning for the hold note.
		//
		
		// Delete the note from target of judgement, and so the note is not judged in the next frame and after.
		HOLDBREAK		= 0x10,
		
		// Continue judging the current note also after that frame.
		HOLDCONTINUE	= 0x20,
		
		// This effect is same of HOLDBREAK.
		// HOLDBREAK mean bad judgement, but HOLDFINSHED is good one like BEST.
		HOLDFINISHED	= 0x30
	};
	
	Judgement operator | (const Judgement& L, const Judgement& R);
	
	Judgement operator & (const Judgement& L, const Judgement& R);
	Judgement operator & (const Judgement& L, const int& R);
	
	Judgement getJudge(const Judgement& j);
	
	Judgement getHoldcmd(const Judgement& j);
	
	

	struct JudgeResult : score::Note {
		JudgeResult(const Note &_note, Judgement _result, double _error) noexcept
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
			judgefunc_return_t(Judgement _judge, int _indexInLane)
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



		TimingJudge(
			size_t numofKeys
		) noexcept;
	
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
		std::array<notes_t, score::numofLanes> notes;
		std::array<notes_t::const_iterator, score::numofLanes> enumBegNote;

		beg_judge_func_t begJudgeFunc;
		end_judge_func_t endJudgeFunc;
		missed_judge_func_t missedJudgeFunc;
	
		double enumRangeSec;

		std::array<const score::Note*, 4> judgingNote;
		
		std::vector<JudgeResult> results;
		
		Controller controller;

		void initAll();
		void initJudge();
		
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
