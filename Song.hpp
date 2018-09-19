/*

This class manages the Score and Header of song.

2018/9/12 written by Nanami Yamamoto

*/


#ifndef _SONG_HPP_
#define _SONG_HPP_


#include <string>
#include <vector>

#include "Fraction.hpp"
#include "Note.hpp"

namespace song {

	class Song {

		

		enum class Genre {
			ORIGINAL = 0,
			VOCALOID = 1,
			J_POP = 2,
			ANIME = 3,
			GAME = 4,
			VARIETY = 5
		};

		enum class Difficulty {
			Easy = 0,
			Normal = 1,
			Hard = 2
		};



		struct Header {
			Header(std::string _title, std::string _artist, Genre _genre);
			~Header();

			const std::string title;
			const std::string artist;
			const Genre genre;

			std::vector<

		};

	public:
		Song();
		~Song();

		bool load();


	};





	class TickTime {
	public:
		TickTime(const std::vector< ) noexcept;
		~TickTime();

		void set(int barCnt, const math::Fraction &posInBar) noexcept;

	private:
		math::Fraction posInBar;
		int barCnt;
		size_t tickTime;
	};


}


#endif // !_SONG_HPP_
