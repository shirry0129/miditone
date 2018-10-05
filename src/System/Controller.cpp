//
//  Controller.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/04.
//

#include "Controller.hpp"


namespace musicgame {


	Key::Key() noexcept : state(State::UP) {};
	
	Key::Key(bool isPressed) noexcept
		: state((isPressed) ? State::PRESSED_MOMENT : State::UP) {};

	void Key::update(bool _isPressed) noexcept {
		if (_isPressed) {
			toDown();
		} else {
			toUP();
		}
	}

	bool Key::isOn() const noexcept {
		switch (state) {
		  case State::PRESSED_MOMENT:
		  case State::DOWN:
			return true;
		  case State::RELEASED_MOMENT:
		  case State::UP:
			return false;
		}
	}

	bool Key::isOff() const noexcept {
		return !isOn();
	};

	bool Key::isChanged() const noexcept {
		switch (state) {
		  case State::PRESSED_MOMENT:
		  case State::RELEASED_MOMENT:
			return true;
		  case State::UP:
		  case State::DOWN:
			return false;
		}
	}

	bool Key::isPressedMoment() const noexcept {
		if (state == State::PRESSED_MOMENT)
			return true;
		else
			return false;
	}

	bool Key::isReleasedMoment() const noexcept {
		if (state == State::RELEASED_MOMENT)
			return true;
		else
			return false;
	}

	void Key::toDown() noexcept {
		switch (state) {
		  case State::PRESSED_MOMENT:
			state = State::DOWN;
			break;
		  case State::DOWN:
			break;
		  case State::RELEASED_MOMENT:
		  case State::UP:
			state = State::PRESSED_MOMENT;
		}
	}

	void Key::toUP() noexcept {
		switch (state) {
		  case State::PRESSED_MOMENT:
		  case State::DOWN:
			state = State::RELEASED_MOMENT;
			break;
		  case State::RELEASED_MOMENT:
			state = State::UP;
		  case State::UP:
			break;
		}
	}
	
	
	Controller::Controller(size_t numofKeys) {
		create(numofKeys);
	};
	
	Controller::Controller() {};
	
	Controller::~Controller() {};
	
	bool Controller::create(size_t numofKeys) {
		if (numofKeys == 0) return false;
		
		for (int i = 0; i < numofKeys; i++)
			key.push_back(Key());
		
		return true;
    }
    
    musicgame::Key &Controller::getKey(size_t keyNum) {
		return key.at(keyNum);
    }
    

}
