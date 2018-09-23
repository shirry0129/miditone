/*

This class retains all note data of a difficulty mode

author	: Nanami Yamamoto
date	: 2018/9/20

*/


#ifndef _SCORE_HPP_
#define _SCORE_HPP_

#include "ScoreReader.hpp"


namespace score {

	constexpr size_t numofLanes = 4;


	enum class Difficulty {
		EASY = 0,
		NORMAL,
		HARD
	};

	enum class NoteType {
		HIT = 0,
		HOLD = 1
	};

	struct NoteTime {
		NoteTime(int _bar, double _msec) noexcept
			: bar(_bar), msec(_msec) {};
		
		const int bar;
		const double msec;
	};

	struct Note {
		Note(
			NoteType _type, int _lane, int _index, 
			const NoteTime &time_begin, const NoteTime &time_end) noexcept
			: type(_type), lane(_lane), index(_index), t_beg(time_begin), t_end(time_end) {}
		
		const NoteTime t_beg;
		const NoteTime t_end;
		const NoteType type;
		const int lane;
		const int index;
	};

	struct Tempo {
		Tempo(const NoteTime &_time, float _tempo) noexcept
			: time(_time), tempo(_tempo) {}
		
		const NoteTime time;
		const float tempo;
	};

	struct Beat {
		Beat(const NoteTime &_time, const math::Fraction &_beat) noexcept
			: time(_time), beat(_beat) {}

		const NoteTime time;
		const math::Fraction beat;
	};




	class Score {
	public:
		struct Header {
			std::basic_string<char_type> title;
			std::basic_string<char_type> artist;
			std::basic_string<char_type> level;
			Difficulty difficulty;

			void clear() {
				title.clear();
				artist.clear();
				level.clear();
				difficulty = Difficulty::EASY;
			}
		};

	
		Score() noexcept;
		Score(const char *file, Difficulty difficulty);
		~Score();

		bool create(const char *file, Difficulty difficulty);
		bool recreate();

		inline int getNumofBars() const noexcept;
		inline int getNumofHolds() const noexcept;
		inline int getNumofHits() const noexcept;

		const std::vector<Tempo>& getTempo() const noexcept;
		float getTempo(double msec) const noexcept;

		const std::vector<Beat>& getBeat() const noexcept;
		math::Fraction getBeat(double msec) const noexcept;

		const Score::Header& getHeader() const noexcept;

	private:
		std::vector<Note> notes;

		std::basic_string<char_type> path;

		int numofBars;
		int numofHolds;
		int numofHits;
		Score::Header header;

		std::vector<Tempo> tempo;
		std::vector<Beat> beat;

		void init();

	};


}

#endif // !_SCORE_HPP_
