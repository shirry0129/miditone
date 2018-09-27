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


	struct ScoreTime {
		ScoreTime(int _bar, const math::Fraction &_posInBar = math::Fraction(0)) :
			bar(_bar), posInBar(_posInBar) {
			barLength = bar - 1 + posInBar;
		}

		// origin is zero
		const math::Fraction& getBarLength() const noexcept { return barLength; }

		const int bar;
		const math::Fraction posInBar;

	private:
		math::Fraction barLength;
	};

	struct TempoEvent : ScoreTime {
		TempoEvent(float _tempo, int _bar, const math::Fraction &_posInBar) noexcept
			: ScoreTime(_bar, _posInBar), tempo(_tempo) {}

		const float tempo;
	};

	struct BeatEvent : ScoreTime {
		BeatEvent(const math::Fraction &_beat, int _bar) noexcept
			: ScoreTime(_bar, math::Fraction(0)), beat(_beat) {}

		const math::Fraction beat;
	};

	struct NoteEvent : ScoreTime {
		NoteEvent(int _type, int _lane, int _bar, const math::Fraction &_posInBar)
			: ScoreTime(_bar, _posInBar), lane(_lane), type(_type) {}

		const int lane;
		const int type;
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
		std::vector<TempoEvent>			tempo;
		std::vector<BeatEvent>			beat;
	};



	class ScoreReader {
	public:
		static constexpr size_t buffer_size = 256;

		enum class State {
			S_REACH_CHUNK_END = 1,
			S_OK = 0,
			E_CANNOT_OPEN_FILE = -1,		// when open file
			E_CANNOT_READ_WHOLELINE = -2,	// when read file (move file pointer)
			E_CANNOT_FIND_COMMAND = -3,		// when read file (move file pointer)
			E_UNEXPECTED_STRING = -4,		// when read header or note data
			E_SET_NOFILE = -5,				// after constructed
			E_EMBED_NO_BEAT_OR_TEMPO = -6,	// when read header
			E_CANNOT_FIND_CHUNK = -7		// when move file pointer
		};

		static bool success(State s);
		static bool failed(State s);
		static bool readable(State s);
		
	
		ScoreReader();
		ScoreReader(const wchar_t *file, char_type delim = ':');
		ScoreReader(const char *file, char_type delim = ':');
		~ScoreReader();

		State open(const wchar_t *file);
		State open(const char * file);


		void setDelim(char_type delim) noexcept;

		State readHeader(Header &header, const std::basic_string<char_type> &chunkName);

		State readNote(std::vector<NoteEvent> &notes, const std::basic_string<char_type> &chunkName);


	private:
		std::basic_ifstream<char_type> score;

		Header header;
		std::vector<NoteEvent> notes;

		State prevState;
		// name of chunk where exist current file pointer
		std::basic_string<char_type> currentChunk;
		// whether process arguments
		bool argProcessFlag;
		char_type delim;
		
		std::array<char_type, buffer_size> buffer;


		void init();
		State processLine();
		State moveToBegin();
		// move file pointer
		State moveChunk(const std::basic_string<char_type> &chunkName);
		


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

		struct NoteCmd : Command {
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
			NoteCmd			cmd_note;
			NullCmd			cmd_null;

			bool isNumber(const std::basic_string<char_type> &str);

		} cmdMng;


	
	};

}

#endif // !_SCOREREADER_HPP_


