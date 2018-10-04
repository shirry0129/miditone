//
//  Controller.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/04.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <vector>


namespace musicgame {

	class Key {
	
		enum class State {
			PRESSED_MOMENT,
			DOWN,
			RELEASED_MOMENT,
			UP
		};

	public:
		Key() noexcept;
		Key(bool isPressed) noexcept;

		void update(bool _isPressed) noexcept;
		
		bool isOn() const noexcept;

		bool isOff() const noexcept;
		
		bool isChanged() const noexcept;
		
		bool isPressedMoment() const noexcept;
		
		bool isReleasedMoment() const noexcept;
		
	private:
		void toDown() noexcept;
		
		void toUP() noexcept;
		
		State state;

	};


	class Controller {
	public:
		Controller(size_t numofKeys);
		Controller();
		~Controller();
		
		bool create(size_t numofKeys);
		
		Key& getKey(size_t keyNum);
		
	private:
		std::vector<Key> key;
	};


}

#endif /* Controller_hpp */
