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

	std::basic_string<Judgement::char_type> Judgement::getJudgeMsg() const noexcept{
		return createJudgeMsg(this->enumVal);
	}

	std::basic_string<Judgement::char_type> Judgement::getHoldMsg() const noexcept{
		return createHoldMsg(this->enumVal);
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
			str += U" ";
		
		str += createHoldMsg(val);
		
		return str;
	}

	std::basic_string<Judgement::char_type> Judgement::createJudgeMsg(JudgeState val) {
		using namespace musicgame;
		std::basic_string<char_type> str;
		
		JudgeState v = val & 0x0f;
		switch (v) {
			case JudgeState::NONE:
				str += U"JudgeState::NONE";
				break;
			case JudgeState::BEST:
				str += U"JudgeState::BEST";
				break;
			case JudgeState::BETTER:
				str += U"JudgeState::BETTER";
				break;
			case JudgeState::GOOD:
				str += U"JudgeState::GOOD";
				break;
			case JudgeState::NOTBAD:
				str += U"JudgeState::NOTBAD";
				break;
			case JudgeState::BAD:
				str += U"JudgeState::BAD";
				break;
			case JudgeState::MISS:
				str += U"JudgeState::MISS";
				break;
			default:
				break;
		}
		
		return str;
	}

	std::basic_string<Judgement::char_type> Judgement::createHoldMsg(JudgeState val) {
		using namespace musicgame;
		std::basic_string<char_type> str;
		
		JudgeState v = val & 0xf0;
		switch (v) {
		  case JudgeState::HOLDBREAK:
		  	str += U"JudgeState::HOLDBREAK";
		  	break;
		  case JudgeState::HOLDCONTINUE:
		  	str += U"JudgeState::HOLDCONTINUE";
		  	break;
		  case JudgeState::HOLDFINISHED:
		  	str += U"JudgeState::HOLDFINISHED";
		  	break;
		  default:
		  	break;
		}
		
		return str;
	}


}
