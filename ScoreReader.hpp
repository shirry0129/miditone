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


	enum class Status {
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

	bool success(Status s);
	bool failed(Status s);
	bool readable(Status s);


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

		Status open(const wchar_t *file);
		Status open(const char * file);


		void setDelim(char_type delim) noexcept;

		// move file pointer
		Status moveChunk(const std::basic_string<char_type> &chunkName);

		Status readHeader(Header &header);

		Status readNote();


	private:
		std::basic_ifstream<char_type> score;

		Header header;

		Status prevStatus;

		// name of chunk where exist current file pointer
		std::basic_string<char_type> currentChunk;

		// whether process arguments
		bool argProcessFlag;
		
		char_type delim;
		
		std::array<char_type, 256> buffer;


		void init();
	
		Status processLine();

		Status moveToBegin();

		


		struct Command {
			virtual Status execute(ScoreReader &sr, const char_type *line) = 0;
		};

		struct BeginCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct EndCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct IdCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct TitleCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct ArtistCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct GenreCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct LevelCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct TempoCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct BeatCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
		};

		struct NullCmd : Command {
			Status execute(ScoreReader &sr, const char_type *line) override;
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


