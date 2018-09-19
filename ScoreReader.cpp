#include "ScoreReader.hpp"

#include <sstream>
#include <ctype.h>


std::locale::id std::codecvt<char32_t, char, std::mbstate_t>::id;



namespace score {


	bool success(Status s) {
		return static_cast<int>(s) >= 0;
	}

	bool failed(Status s) {
		return static_cast<int>(s) < 0;
	}

	bool readable(Status s) {
		return (s != Status::E_SET_NOFILE && s != Status::E_CANNOT_OPEN_FILE);
	}



	ScoreReader::ScoreReader() {
		init();
	}

	ScoreReader::ScoreReader(const wchar_t *file, char_type delim) {
		prevStatus = open(file);
		this->delim = delim;
	}

	ScoreReader::ScoreReader(const char *file, char_type delim) {
		prevStatus = open(file);
		this->delim = delim;
	}

	ScoreReader::~ScoreReader() {}

	Status ScoreReader::open(const wchar_t * file) {
		char path[256];
		wcstombs(path, file, 256);
		return open(path);
	}

	Status ScoreReader::open(const char *file) {
		// init
		char_type d = delim;
		init();
		delim = d;


		score.open(file);

		if (!score) {
			return prevStatus = Status::E_CANNOT_OPEN_FILE;
		}

		return prevStatus = Status::S_OK;
	}

	Status ScoreReader::moveChunk(const std::basic_string<char_type> &chunkName) {
		if (!readable(prevStatus))
			return prevStatus = Status::E_SET_NOFILE;


		// move file pointer to begin
		moveToBegin();

		// don't process arguments
		argProcessFlag = false;

		while (!score.eof() && currentChunk != chunkName) {
			if (failed(processLine()))
				return prevStatus;
		}

		// reach eof ?
		if (score.eof()) {
			score.clear(score.rdstate() & ~std::ios_base::eofbit); // unset flag of eof
			return prevStatus = Status::E_CANNOT_FIND_CHUNK;
		}

		return prevStatus = Status::S_OK;
	}

	Status ScoreReader::readHeader(Header &_header, const std::basic_string<char_type> &chunkName) {
		if (!readable(prevStatus))
			return prevStatus;

		// move file pointer
		if (failed(moveChunk(chunkName)))
			return prevStatus;

		// process all arguments
		argProcessFlag = true;

		while (!score.eof() && currentChunk == chunkName) {
			if (failed(processLine()))
				return prevStatus;
		}

		// reach eof ?
		if (score.eof()) {
			score.clear(score.rdstate() & ~std::ios_base::eofbit); // unset flag of eof
		}

		// exist any tempo data or beat data ?
		if (header.tempo.empty() || header.beat.empty())
			return prevStatus = Status::E_EMBED_NO_BEAT_OR_TEMPO;


		_header = std::move(header);

		return prevStatus = Status::S_OK;
	}

	Status ScoreReader::readNote() {


		return prevStatus = Status::S_OK;
	}

	void score::ScoreReader::init() {

		if (score.is_open())
			score.close();

		currentChunk.erase(currentChunk.cbegin(), currentChunk.cend());
		prevStatus = Status::E_SET_NOFILE;
		delim = U':';
		argProcessFlag = false;

#define eraseAll(container) container.erase(container.cbegin(), container.cend())

		header.id = 0;
		eraseAll(header.title);
		eraseAll(header.artist);
		for (auto &l : header.level) eraseAll(l);
		eraseAll(header.genre);
		header.tempo.clear();
		header.beat.clear();

#undef eraseAll

	}

	Status ScoreReader::processLine() {
		// read a line
		score.getline(buffer.data(), buffer.size());

		// fstream can have read a whole line ? 
		if (score.fail()) {
			score.clear(score.rdstate() & ~std::ios_base::failbit); // unset flag of fail
			return prevStatus = Status::E_TOOLONG_DATALINE;
		}

		// get command object
		Command *cmd = cmdMng.inputHandler(buffer.data(), delim);
		if (cmd == nullptr)
			return prevStatus = Status::E_CANNOT_FIND_COMMAND;

		// execute
		if (failed(cmd->execute(*this, buffer.data())))
			return prevStatus;


		return prevStatus = Status::S_OK;
	}

	void score::ScoreReader::setDelim(char_type _delim) noexcept {
		delim = _delim;
	}

	Status score::ScoreReader::moveToBegin() {
		if (failed(prevStatus))
			return prevStatus;

		score.seekg(0, std::ios_base::beg);
		currentChunk = "";

		argProcessFlag = false;

		return prevStatus = Status::S_OK;
	}





	Status ScoreReader::BeginCmd::execute(ScoreReader &sr, const char_type *line) {

		std::basic_stringstream<char_type> sstream(line);

		std::array<char_type, 32> section;

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		sstream.getline(section.data(), section.size(), sr.delim);
		std::basic_string<char_type> firstArg(section.data());
		if (firstArg == "")
			return sr.prevStatus = Status::E_INVALID_ARGUMENT;


		// update current chunk name
		sr.currentChunk = firstArg;


		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::EndCmd::execute(ScoreReader &sr, const char_type *line) {
		
		// update current chunk name
		sr.currentChunk = "";
		
		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::IdCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string as int
		sstream >> sr.header.id;

		if (sstream.fail())
			return sr.prevStatus = Status::E_INVALID_ARGUMENT;

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::TitleCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		char_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.title.assign(tmp);

		if (sstream.fail())
			return sr.prevStatus = Status::E_INVALID_ARGUMENT;

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::ArtistCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		char_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.artist.assign(tmp);

		if (sstream.fail())
			return sr.prevStatus = Status::E_INVALID_ARGUMENT;

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::GenreCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		char_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.genre.assign(tmp);

		if (sstream.fail())
			return sr.prevStatus = Status::E_INVALID_ARGUMENT;

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::LevelCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument strings
		std::array<char_type, 4> tmp;
		for (auto it = sr.header.level.begin(); sstream.good() && it != sr.header.level.end(); it++) {
			sstream.getline(tmp.data(), tmp.size(), sr.delim);
			it->assign(tmp.data());

			if (sstream.fail())
				return sr.prevStatus = Status::E_INVALID_ARGUMENT;
		}

		
		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::TempoCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument string
		char_type tmp;
		math::Fraction posInBar;
		float tempo;
		int bar;
		int n, d;

		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 0: sstream >> bar; break;
			case 1: sstream >> n; break;
			case 2: sstream >> d; break;
			case 3: sstream >> tempo;
			}

			if (sstream.fail())
				return sr.prevStatus = Status::E_INVALID_ARGUMENT;

			// read delimiter
			sstream >> tmp;
		}

		// add tempo data
		sr.header.tempo.emplace_back(TempoEvent(tempo, bar, math::Fraction(n, d)));

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::BeatCmd::execute(ScoreReader & sr, const char_type * line) {
		if (!sr.argProcessFlag)
			return Status::S_OK;	// skip

		std::basic_stringstream<char_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument string
		char_type tmp;
		int bar;
		int n, d;

		for (int i = 0; i < 3; i++) {
			switch (i) {
			case 0: sstream >> bar; break;
			case 1: sstream >> n; break;
			case 2: sstream >> d; break;
			}

			if (sstream.fail())
				return sr.prevStatus = Status::E_INVALID_ARGUMENT;

			// read delimiter
			sstream >> tmp;
		}

		// add beat data
		sr.header.beat.emplace_back(BeatEvent(math::Fraction(n, d), bar, math::Fraction(0)));

		return sr.prevStatus = Status::S_OK;
	}

	Status ScoreReader::NullCmd::execute(ScoreReader & sr, const char_type * line) {
		return sr.prevStatus = Status::S_OK;
	}

	ScoreReader::Command * ScoreReader::CommandManager::inputHandler(const char_type * line, char_type delim) {

		std::basic_stringstream<char_type> sstream(line);

		std::array<char_type, 32> section;

		// get command string 
		sstream.getline(section.data(), section.size(), delim);
		std::basic_string<char_type> cmdStr(section.data());

		if (cmdStr == "begin")
			return &cmd_begin;
		if (cmdStr == "end")
			return &cmd_end;
		if (cmdStr == "id")
			return &cmd_id;
		if (cmdStr == "title")
			return &cmd_title;
		if (cmdStr == "artist")
			return &cmd_artist;
		if (cmdStr == "genre")
			return &cmd_genre;
		if (cmdStr == "level")
			return &cmd_level;
		if (cmdStr == "tempo")
			return &cmd_tempo;
		if (cmdStr == "beat")
			return &cmd_beat;
		if (cmdStr == "")
			return &cmd_null;

		return nullptr;
	}

	

}
