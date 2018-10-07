//
//  Judgement.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/07.
//

#include "Judgement.hpp"

namespace musicgame {


	JudgeState operator | (const JudgeState& L, const JudgeState& R) {
		return static_cast<JudgeState>(
			static_cast<int>(L) | static_cast<int>(R)
		);
	}
	
	JudgeState operator | (const JudgeState& L, const int& R) {
		return static_cast<JudgeState>(
			static_cast<int>(L) | R
		);
	}
	
	JudgeState operator & (const JudgeState& L, const JudgeState& R) {
		return static_cast<JudgeState>(
			static_cast<int>(L) & static_cast<int>(R)
		);
	}
	
	JudgeState operator & (const JudgeState& L, const int& R) {
		auto t = static_cast<JudgeState>(
			static_cast<int>(L) & R
		);
		
		return t;
	}
	
	
	Judgement::Judgement(JudgeState state) noexcept
	: Enum<JudgeState>(state, createErrMsg) {};

	Judgement::Judgement() noexcept
	: Enum<JudgeState>(JudgeState::NONE, createErrMsg) {};

	JudgeState Judgement::getJudge() const noexcept {
		return this->enumVal & 0x0f;
	}

	JudgeState Judgement::getHoldState() const noexcept {
		return this->enumVal & 0xf0;
	}

	std::basic_string<Judgement::char_type> Judgement::getJudgeMsg(JudgeState val) {
		return createJudgeMsg(val);
	}

	std::basic_string<Judgement::char_type> Judgement::getHoldMsg(JudgeState val) {
		return createHoldMsg(val);
	}

	bool Judgement::isContinueHold() const noexcept {
		return getHoldState() == JudgeState::HOLDCONTINUE;
	}

	bool Judgement::isJudged() const noexcept {
		return this->enumVal != JudgeState::NONE;
	}
	
	bool Judgement::isMiss() const noexcept {
		return getJudge() == JudgeState::MISS;
	}

	std::basic_string<Judgement::char_type> Judgement::createErrMsg(JudgeState val) {
		using namespace musicgame;
		std::basic_string<char_type> str;
		
		str += createJudgeMsg(val);
		
		if (static_cast<int>(val & 0xf0) > 0)
			str += " ";
		
		str += createHoldMsg(val);
		
		return str;
	}

	std::basic_string<Judgement::char_type> Judgement::createJudgeMsg(JudgeState val) {
		using namespace musicgame;
		std::basic_string<char_type> str;
		#define str(var) #var
		#define enum_case(val) case val: str += str(val); break;
		JudgeState v = val & 0x0f;
		switch (v) {
			enum_case(JudgeState::BEST)
			enum_case(JudgeState::BETTER)
			enum_case(JudgeState::GOOD)
			enum_case(JudgeState::NOTBAD)
			enum_case(JudgeState::BAD)
			enum_case(JudgeState::MISS)
			enum_case(JudgeState::NONE)
			default: break;
		}
		#undef enum_case
		#undef str
		
		return str;
	}

	std::basic_string<Judgement::char_type> Judgement::createHoldMsg(JudgeState val) {
		using namespace musicgame;
		std::basic_string<char_type> str;
		#define str(var) #var
		#define enum_case(val) case val: str += str(val); break;
		JudgeState v = val & 0xf0;
		switch (v) {
		  enum_case(JudgeState::HOLDBREAK)
		  enum_case(JudgeState::HOLDCONTINUE)
		  enum_case(JudgeState::HOLDFINISHED)
		  default: break;
		}
		#undef enum_case
		#undef str
		
		return str;
	}


}
