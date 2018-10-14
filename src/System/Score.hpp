/*

This class retains all note data of a difficulty mode

author	: Nanami Yamamoto
date	: 2018/9/20

*/


#ifndef _SCOREMANAGER_HPP_
#define _SCOREMANAGER_HPP_


#include "Header.hpp"
#include "ScoreTimeConverter.hpp"


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
		NoteTime(int _bar, double _sec) noexcept
			: bar(_bar), sec(_sec) {};
		
		const int bar;
		const double sec;
	};
	

	struct Note {
		Note(
			NoteType _type, int _lane, int _index, int _indexInLane,
			const NoteTime &time_begin, const NoteTime &time_end) noexcept
			: type(_type), lane(_lane), index(_index), indexInLane(_indexInLane),
			t_beg(time_begin), t_end(time_end) {}
		
		const NoteType type;
		const int lane;
		const int index;
		const int indexInLane;
		const NoteTime t_beg;
		const NoteTime t_end;
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
	
	
	struct Bar {
		Bar(const NoteTime &_time, int _barCnt) noexcept
			: time(_time), barCnt(_barCnt) {}

		const NoteTime time;
		const int barCnt;
	};


	class Score {
	public:
		
		enum class State {
			S_OK = 0,
			E_INVALID_ARGUMENT = -1,
			E_INVALID_NOTE = -2,
			E_READER_FAILED = -3,
			E_INVALID_TEMPO_BEAT = -4,
			E_INVALID_LANE = -5
		};
	
		struct Header {
			int								id;
			std::basic_string<char_type>	title;
			std::basic_string<char_type>	artist;
			std::basic_string<char_type>	level;
			Genre							genre;
			Difficulty						difficulty;

			void clear() {
				title.clear();
				artist.clear();
				level.clear();
				difficulty = Difficulty::EASY;
			}
		};

	
		Score() noexcept;
		Score(const std::string &file, Difficulty difficulty);
		~Score();

		Error<State> create(const std::string &file, Difficulty difficulty);
		Error<State> recreate();

		void clear();

		int getNumofBars() const noexcept;
		int getNumofHolds() const noexcept;
		int getNumofHits() const noexcept;
		int getNumofNotes() const noexcept;
		int getNumofLaneNotes(int laneNum) const noexcept;

		const std::vector<Tempo>& getTempo() const noexcept;
		float getTempo(double sec) const noexcept;

		const std::vector<Beat>& getBeat() const noexcept;
		math::Fraction getBeat(double sec) const noexcept;

		const std::vector<Bar>& getBar() const noexcept;
		int getBar(double sec) const noexcept;

		const Score::Header& getHeader() const noexcept;

		const std::vector<Note> &getNotes() const noexcept;

		const score::ScoreTimeConverter &getConverter() const noexcept;
		
		const Error<Score::State> &getLastError() const noexcept;
		
		const ScoreReader &getReader() const noexcept;
		
	private:
	
		ScoreReader reader;
	
		std::vector<Note> notes;

		std::basic_string<char_type> path;

		score::ScoreTimeConverter timeConv;
		
		static std::string createErrMessage(State state);

		int numofBars;
		int numofHolds;
		int numofHits;
		std::array<int, numofLanes> numofNotesInLane;
		

		Score::Header header;

		std::vector<Tempo> tempo;
		std::vector<Beat> beat;
		std::vector<Bar> bar;
		
		Error<Score::State> prevError;
		
		void init();

	};

	using score_err_t = Error<Score::State>;

}

#endif // !_SCOREMANAGER_HPP_
