/*

This class reads the score file of music game

2018/9/14 written by Nanami Yamamoto

*/


#ifndef _SCOREREADER_HPP_
#define _SCOREREADER_HPP_



#include <fstream>
#include <array>
#include <string>
#include <vector>

#include "Fraction.hpp"

namespace score {

	using char_type = char;


	enum class State {
		S_REACH_CHUNK_END = 1,
		S_OK = 0,
		E_CANNOT_OPEN_FILE = -1,		// when open file
		E_TOOLONG_DATALINE = -2,		// when read file (move file pointer)
		E_CANNOT_FIND_COMMAND = -3,		// when read file (move file pointer)
		E_INVALID_ARGUMENT = -4,		// when read header or note data
		E_SET_NOFILE = -5,				// after constructed
		E_EMBED_NO_BEAT_OR_TEMPO = -6,	// when read header
		E_OUT_OF_RANGE_CHUNK = -7,		// after move file pointer
		E_CANNOT_FIND_CHUNK = -8		// when move file pointer
	};

	bool success(State s);
	bool failed(State s);
	bool readable(State s);


	struct ScoreTime {
		ScoreTime(int bar, const math::Fraction &posInBar) :
			bar(bar), posInBar(posInBar) {}
		const int bar;
		const math::Fraction posInBar;
	};

	struct TempoEvent : ScoreTime {
		TempoEvent(float tempo, int bar, const math::Fraction &posInBar) noexcept
			: tempo(tempo), ScoreTime(bar, posInBar) {}

		const float tempo;
	};

	struct BeatEvent : ScoreTime {
		BeatEvent(const math::Fraction &beat, int bar, const math::Fraction &posInBar) noexcept
			: beat(beat), ScoreTime(bar, posInBar) {}

		const math::Fraction beat;
	};

	struct Header {
		int								id;
		std::basic_string<char_type>	title;
		std::basic_string<char_type>	artist;
		std::array<
			std::basic_string<char_type>,
			3
		>								level;
		std::basic_string<char_type>	genre;
		std::vector<TempoEvent>		tempo;
		std::vector<BeatEvent>			beat;
	};



	class ScoreReader {
		
	public:
		ScoreReader();
		ScoreReader(const wchar_t *file, char_type delim = ':');
		ScoreReader(const char *file, char_type delim = ':');
		~ScoreReader();

		State open(const wchar_t *file);
		State open(const char * file);


		void setDelim(char_type delim) noexcept;

		// move file pointer
		State moveChunk(const std::basic_string<char_type> &chunkName);

		State readHeader(Header &header, const std::basic_string<char_type> &chunkName);

		State readNote();


	private:
		std::basic_ifstream<char_type> score;

		Header header;

		State prevStatus;

		// name of chunk where exist current file pointer
		std::basic_string<char_type> currentChunk;

		// whether process arguments
		bool argProcessFlag;
		
		char_type delim;
		
		std::array<char_type, 256> buffer;


		void init();
	
		State processLine();

		State moveToBegin();

		


		struct Command {
			virtual State execute(ScoreReader &sr, const char_type *line) = 0;
		};

		struct BeginCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct EndCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct IdCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct TitleCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct ArtistCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct GenreCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct LevelCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct TempoCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct BeatCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct NullCmd : Command {
			State execute(ScoreReader &sr, const char_type *line) override;
		};

		struct CommandManager {
			Command* inputHandler(const char_type *line, char_type delim);
		private:
			BeginCmd		cmd_begin;
			EndCmd			cmd_end;
			IdCmd			cmd_id;
			TitleCmd		cmd_title;
			ArtistCmd		cmd_artist;
			GenreCmd		cmd_genre;
			LevelCmd		cmd_level;
			TempoCmd		cmd_tempo;
			BeatCmd			cmd_beat;
			NullCmd			cmd_null;
		} cmdMng;


	
	};

}

#endif // !_SCOREREADER_HPP_


