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
#include <filesystem>

#include "Fraction.hpp"
#include "Error.hpp"


namespace score {


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

	
	
	class ScoreReader {
	public:
		static constexpr size_t buffer_size = 1050;
		
		using rch_type = char;
		
		
		struct Header {
			int								id;
			std::basic_string<rch_type>		title;
			std::basic_string<rch_type>		artist;
			std::array<
				std::basic_string<rch_type>,
				3
			>								level;
			std::basic_string<rch_type>		genre;
			std::vector<TempoEvent>			tempo;
			std::vector<BeatEvent>			beat;
			int								chorusBegSec;
			int								chorusEndSec;
		};
		

		enum class State {
			S_REACH_CHUNK_END = 1,
			S_OK = 0,
			E_CANNOT_OPEN_FILE = -1,		// when open file
			E_CANNOT_READ_WHOLELINE = -2,	// when read file (move file pointer)
			E_CANNOT_FIND_COMMAND = -3,		// when read file (move file pointer)
			E_UNEXPECTED_STRING = -4,		// when read header or note data
			E_SET_NOFILE = -5,				// after constructed
			E_EMBED_NO_BEAT_OR_TEMPO = -6,	// when read header
			E_CANNOT_FIND_CHUNK = -7,		// when move file pointer
		};

		static bool success(Error<State> s);
		static bool failed(Error<State> s);
		static bool readable(Error<State> s);
		
	
		ScoreReader();
		ScoreReader(const std::filesystem::path& file, rch_type delim = ':');
		~ScoreReader();

		Error<State> open(const std::filesystem::path& file);


		void setDelim(rch_type delim) noexcept;

		Error<State> readHeader(Header &header, const std::basic_string<rch_type> &chunkName);

		Error<State> readNote(std::vector<NoteEvent> &notes, const std::basic_string<rch_type> &chunkName);
		
		/**
		  最後に読み込んだ行の行数を返す

		  @return 行数
		*/
		size_t getCurrentLine() const noexcept;
		
		
		/**
		 現在読み込んでいるチャンク名を返す

		 @return チャンク名(begin:***の***の部分)を返す
		 */
		std::basic_string<rch_type> getCurrentChunk() const noexcept;
		
		
		/**
		 最後に発生したエラーを返す
		 note:成功もエラーの一部になります

		 @return エラーを格納したErrorのオブジェクト
		 */
		const Error<State> &getLastError() const noexcept;
		
		
		/**
		 現在読み込んでいる1行の内容を返す

		 @return 譜面ファイル内のどこかの1行
		 */
		const std::array<rch_type, buffer_size> &getBuffer() const noexcept;

	private:
		std::basic_ifstream<rch_type> score;

		Header header;
		std::vector<NoteEvent> notes;

		Error<State> prevState;
		// name of chunk where exist current file pointer
		std::basic_string<rch_type> currentChunk;
		
		size_t currentLine;
		
		// whether process arguments
		bool argProcessFlag;
		rch_type delim;
		
		std::array<rch_type, buffer_size> buffer;
		
		
		void init();
		Error<State> processLine();
		Error<State> moveToBegin();
		// move file pointer
		Error<State> moveChunk(const std::basic_string<rch_type> &chunkName);
		
		bool isFailReadLine(const std::basic_ifstream<rch_type> &ifs);
		
		void skipBOM();
		
		static std::basic_string<char32_t> createErrMessage(State state);


		struct Command {
			virtual Error<State> execute(ScoreReader &sr, const rch_type *line) = 0;
		};

		struct BeginCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct EndCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct IdCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct TitleCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct ArtistCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct GenreCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct LevelCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct TempoCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct BeatCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct NoteCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};
		
		struct ChobegCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};
		
		struct ChoendCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct NullCmd : Command {
			Error<State> execute(ScoreReader &sr, const rch_type *line) override;
		};

		struct CommandManager {
			Command* inputHandler(const rch_type *line, rch_type delim);
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
			ChobegCmd		cmd_chobeg;
			ChoendCmd		cmd_choend;
			NullCmd			cmd_null;

			bool isNumber(const std::basic_string<rch_type> &str);

		} cmdMng;

	};
	
	using scorereader_err_t = Error<ScoreReader::State>;

}

#endif // !_SCOREREADER_HPP_
