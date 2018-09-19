/*

This class retains all notes included in the score of one difficulty.

2018/9/12 written by Nanami Yamamoto

*/


#ifndef _SCORE_HPP_
#define _SCORE_HPP_


#include <vector>
#include <string>
#include "Note.hpp"

namespace score {




	


	class Score {
	public:
		Score();
		~Score();

		Status load(const std::string &file, Difficulty diff);

		const std::vector<Note> &getNotes() const noexcept;


	private:
		std::vector<Note> notes;

	


	};


}

#endif // !_SCORE_HPP_
