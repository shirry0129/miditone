//
//  Judgement.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/07.
//

#ifndef Judgement_hpp
#define Judgement_hpp

#include "Enum.hpp"


namespace musicgame {


	enum class JudgeState {
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
	
	JudgeState operator | (const JudgeState& L, const JudgeState& R);
	JudgeState operator | (const JudgeState& L, const int& R);
	JudgeState operator & (const JudgeState& L, const JudgeState& R);
	JudgeState operator & (const JudgeState& L, const int& R);
	



	class Judgement : public Enum<JudgeState> {
	
	using char_type = char;
	using create_errmsg_func_t = std::function<std::basic_string<char_type>(JudgeState)>;
	
	public:
		Judgement(JudgeState state) noexcept;
		
		Judgement() noexcept;
	
		JudgeState getJudge() const noexcept;
		
		JudgeState getHoldState() const noexcept;
		
		std::basic_string<char_type> getJudgeMsg(JudgeState val);
		
		std::basic_string<char_type> getHoldMsg(JudgeState val);
		
		bool isContinueHold() const noexcept;
		
		bool isJudged() const noexcept;
		
		bool isMiss() const noexcept;
		
	private:
		
		static std::basic_string<char_type> createErrMsg(JudgeState val);
		
		static std::basic_string<char_type> createJudgeMsg(JudgeState val);
		
		static std::basic_string<char_type> createHoldMsg(JudgeState val);
		
	};

}

#endif /* Judgement_hpp */
