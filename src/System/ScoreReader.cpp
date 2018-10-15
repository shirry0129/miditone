#include "ScoreReader.hpp"

#include <sstream>
#include <ctype.h>


//std::locale::id std::codecvt<char32_t, char, std::mbstate_t>::id;



namespace score {


	bool ScoreReader::success(Error<State> s) {
		return s.getCode() >= 0;
	}

	bool ScoreReader::failed(Error<State> s) {
		return s.getCode() < 0;
	}

	bool ScoreReader::readable(Error<State> s) {
		return (s.get() != State::E_SET_NOFILE && s.get() != State::E_CANNOT_OPEN_FILE);
	}

	


	ScoreReader::ScoreReader()
		: prevState(State::E_SET_NOFILE, createErrMessage) {
		init();
	}

	ScoreReader::ScoreReader(const char *file, rch_type delim)
		: prevState(State::E_SET_NOFILE, createErrMessage)  {
		prevState = open(file);
		this->delim = delim;
	}

	ScoreReader::~ScoreReader() {}

	Error<ScoreReader::State> ScoreReader::open(const char *file) {
		// init
		rch_type d = delim;
		init();
		delim = d;


		score.open(file);

		if (!score) {
			return prevState = State::E_CANNOT_OPEN_FILE;
		}

		return prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::moveChunk(const std::basic_string<rch_type> &chunkName) {
		if (!readable(prevState.get()))
			return prevState = State::E_SET_NOFILE;


		// move file pointer to begin
		moveToBegin();

		// don't process arguments
		argProcessFlag = false;

		while (!score.eof() && currentChunk != chunkName) {
			if (failed(processLine()))
				return prevState;
		}

		// reach eof ?
		if (score.eof()) {
			score.clear(score.rdstate() & ~std::ios_base::eofbit); // unset flag of eof
			return prevState = State::E_CANNOT_FIND_CHUNK;
		}

		return prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::readHeader(Header &_header, const std::basic_string<rch_type> &chunkName) {
		if (!readable(prevState))
			return prevState;

		// move file pointer
		if (failed(moveChunk(chunkName)))
			return prevState;

		// process all arguments
		argProcessFlag = true;

		// read header chunk
		while (!score.eof() && prevState != State::S_REACH_CHUNK_END) {
			if (failed(processLine()))
				return prevState;
		}

		// reach eof ?
		if (score.eof()) {
			score.clear(score.rdstate() & ~std::ios_base::eofbit); // unset flag of eof
		}

		// exist any tempo data or beat data ?
		if (header.tempo.empty() || header.beat.empty())
			return prevState = State::E_EMBED_NO_BEAT_OR_TEMPO;


		_header = std::move(header);

		return prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::readNote(std::vector<NoteEvent> &_notes, const std::basic_string<rch_type> &chunkName) {
		if (!readable(prevState))
			return prevState;

		// move file pointer
		if (failed(moveChunk(chunkName)))
			return prevState;

		// process all arguments
		argProcessFlag = true;

		// read Note data
		while (!score.eof() && prevState != State::S_REACH_CHUNK_END) {
			if (failed(processLine()))
				return prevState;
		}

		// reach eof ?
		if (score.eof()) {
			score.clear(score.rdstate() & ~std::ios_base::eofbit); // unset flag of eof
		}

		_notes = std::move(notes);

		return prevState = State::S_OK;
	}
	
	size_t ScoreReader::getCurrentLine() const noexcept {
		return currentLine;
	}
	
	std::basic_string<ScoreReader::rch_type> ScoreReader::getCurrentChunk() const noexcept {
		return currentChunk;
	}
	
	const Error<ScoreReader::State> &ScoreReader::getLastError() const noexcept {
		return prevState;
	}
	
	const std::array<ScoreReader::rch_type, ScoreReader::buffer_size> &ScoreReader::getBuffer() const noexcept {
		return buffer;
	}

	void score::ScoreReader::init() {

		if (score.is_open())
			score.close();

		buffer.fill('\0');
		currentChunk.erase(currentChunk.cbegin(), currentChunk.cend());
		currentLine = 0;
		prevState = State::E_SET_NOFILE;
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

	Error<ScoreReader::State> ScoreReader::processLine() {
		currentLine++;
	
		// read a line
		score.getline(buffer.data(), buffer.size());
		
		// fstream can have read a whole line ?
		if (isFailReadLine(score)) {
			score.clear(score.rdstate() & ~std::ios_base::failbit); // unset flag of fail
			return prevState = State::E_CANNOT_READ_WHOLELINE;
		}
		
		// remove CR
		auto strEnd = score.gcount() - 2;
		if (strEnd >= 0 && buffer.at(strEnd) == '\r')
			buffer.at(strEnd) = '\0';

		// get command object
		Command *cmd = cmdMng.inputHandler(buffer.data(), delim);
		if (cmd == nullptr)
			return prevState = State::E_CANNOT_FIND_COMMAND;

		// execute
		cmd->execute(*this, buffer.data());
		return prevState;
	}

	void score::ScoreReader::setDelim(rch_type _delim) noexcept {
		delim = _delim;
	}

	Error<ScoreReader::State> score::ScoreReader::moveToBegin() {
		if (failed(prevState))
			return prevState;

		score.seekg(0, std::ios_base::beg);
		
		skipBOM();
		
		currentChunk = "";
		currentLine = 0;

		argProcessFlag = false;

		return prevState = State::S_OK;
	}
	
	std::basic_string<char32_t> ScoreReader::createErrMessage(State state) {
		std::basic_string<char32_t> msg;
	
		switch (state) {
		  case State::S_REACH_CHUNK_END:
			msg += U"チャンクの終端に達しました";
			return msg;
		  case State::S_OK:
		  	msg += U"成功";
		  	return msg;
		  case State::E_CANNOT_OPEN_FILE:
		  	msg += U"ファイルを開けません";
		  	return msg;
		  case State::E_CANNOT_READ_WHOLELINE:
		  	msg += U"1行が長すぎます";
		  	return msg;
		  case State::E_CANNOT_FIND_COMMAND:
		  	msg += U"コマンドが見つかりませんでした";
		  	return msg;
		  case State::E_UNEXPECTED_STRING:
		  	msg += U"予期せぬ文字列が見つかりました";
		  	return msg;
		  case State::E_SET_NOFILE:
		  	msg += U"ファイルが設定されていません";
		  	return msg;
		  case State::E_EMBED_NO_BEAT_OR_TEMPO:
		  	msg += U"拍子またはテンポ情報が埋め込まれていません";
			return msg;
		  case State::E_CANNOT_FIND_CHUNK:
		    msg += U"難易度またはヘッダが見つかりませんでした";
		    return msg;
		  default:
			return msg;
		}
	}
	
	bool ScoreReader::isFailReadLine(const std::basic_ifstream<rch_type> &ifs) {
		if (!ifs.bad() && !ifs.eof() && ifs.fail())
			return true;
		else
			return false;
	}
	
	void ScoreReader::skipBOM() {
		rch_type s[4];
		
		score.seekg(0, std::ios_base::beg);
		score.getline(s, 4);
		
		if (score.fail()) {
			score.clear(score.rdstate() & ~std::ios_base::failbit); // unset flag of fail
		}
		
		std::basic_string<rch_type> bom(s);
	
		if (bom == "\xef\xbb\xbf") {
			// UTF-8 with BOM
			score.seekg(3, std::ios_base::beg);
		} else if (bom == "\xfe\xff") {
			// UTF-16BE with BOM
			score.seekg(2, std::ios_base::beg);
		} else if (bom == "\xff\xfe") {
			// UTF-16LE with BOM
			score.seekg(2, std::ios_base::beg);
		} else {
			score.seekg(0, std::ios_base::beg);
		}
	}

	Error<ScoreReader::State> ScoreReader::BeginCmd::execute(ScoreReader &sr, const rch_type *line) {
		if (sr.currentChunk != "")
			return State::E_UNEXPECTED_STRING;


		std::basic_stringstream<rch_type> sstream(line);

		std::array<rch_type, 32> section;

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		sstream.getline(section.data(), section.size(), sr.delim);
		std::basic_string<rch_type> firstArg(section.data());
		if (firstArg == "")
			return sr.prevState = State::E_UNEXPECTED_STRING;

		// update current chunk name
		sr.currentChunk = firstArg;


		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::EndCmd::execute(ScoreReader &sr, const rch_type *line) {
		if (sr.prevState == State::S_REACH_CHUNK_END)
			return sr.prevState = State::E_UNEXPECTED_STRING;
		
		// update current chunk name
		sr.currentChunk = "";
		
		return sr.prevState = State::S_REACH_CHUNK_END;
	}

	Error<ScoreReader::State> ScoreReader::IdCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string as int
		sstream >> sr.header.id;

		if (sstream.fail())
			return sr.prevState = State::E_UNEXPECTED_STRING;

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::TitleCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		rch_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.title.assign(tmp);

		if (sstream.fail())
			return sr.prevState = State::E_UNEXPECTED_STRING;

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::ArtistCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		rch_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.artist.assign(tmp);

		if (sstream.fail())
			return sr.prevState = State::E_UNEXPECTED_STRING;

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::GenreCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get first argument string
		rch_type tmp[128];
		sstream.getline(tmp, 128);
		sr.header.genre.assign(tmp);

		if (sstream.fail())
			return sr.prevState = State::E_UNEXPECTED_STRING;

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::LevelCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument strings
		std::array<rch_type, 4> tmp;
		for (auto it = sr.header.level.begin(); sstream.good() && it != sr.header.level.end(); it++) {
			sstream.getline(tmp.data(), tmp.size(), sr.delim);
			it->assign(tmp.data());

			if (sstream.fail())
				return sr.prevState = State::E_UNEXPECTED_STRING;
		}

		
		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::TempoCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument string
		rch_type tmp;
		math::Fraction posInBar;
		float tempo = 0.0f;
		int bar = 0;
		int n = 0, d = 1;

		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 0: sstream >> bar; break;
			case 1: sstream >> n; break;
			case 2: sstream >> d; break;
			case 3: sstream >> tempo;
			}

			if (sstream.fail())
				return sr.prevState = State::E_UNEXPECTED_STRING;

			// read delimiter
			sstream >> tmp;
		}

		// add tempo data
		sr.header.tempo.emplace_back(
			tempo, bar, math::Fraction(n, d).reduce()
		);

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::BeatCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// ignore command string 
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), sr.delim);

		// get argument string
		rch_type tmp;
		int bar = 0;
		int n = 0, d = 1;

		for (int i = 0; i < 3; i++) {
			switch (i) {
			case 0: sstream >> bar; break;
			case 1: sstream >> n; break;
			case 2: sstream >> d; break;
			}

			if (sstream.fail())
				return sr.prevState = State::E_UNEXPECTED_STRING;

			// read delimiter
			sstream >> tmp;
		}

		// add beat data
		sr.header.beat.emplace_back(math::Fraction(n, d), bar);

		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::NoteCmd::execute(ScoreReader & sr, const rch_type * line) {
		if (!sr.argProcessFlag)
			return State::S_OK;	// skip

		if (sr.currentChunk == "")
			return State::E_UNEXPECTED_STRING;

		std::basic_stringstream<rch_type> sstream(line);

		// get argument string as int
		rch_type tmp;
		int lane = 0;
		int bar = 0;
		std::basic_string<rch_type> timing;

		for (int i = 0; i < 3; i++) {
			switch (i) {
			case 0: sstream >> lane; break;
			case 1: sstream >> bar; break;
			case 2: sstream >> timing; break;
			}

			if (sstream.fail())
				return sr.prevState = State::E_UNEXPECTED_STRING;

			// read delimiter
			sstream >> tmp;
		}

		// create note data
		int count = 0;
		for (const auto t : timing) {
			int type = t - '0';
			if (type != 0) {
				sr.notes.emplace_back(
					type, lane, bar, math::Fraction(count, static_cast<int>(timing.size())).reduce()
				);
			}
			count++;
		}


		return sr.prevState = State::S_OK;
	}

	Error<ScoreReader::State> ScoreReader::NullCmd::execute(ScoreReader & sr, const rch_type * line) {
		return sr.prevState = State::S_OK;
	}

	ScoreReader::Command * ScoreReader::CommandManager::inputHandler(const rch_type * line, rch_type delim) {

		std::basic_stringstream<rch_type> sstream(line);

		std::array<rch_type, 32> section;

		// get command string 
		sstream.getline(section.data(), section.size(), delim);
		std::basic_string<rch_type> cmdStr(section.data());

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
		if (isNumber(cmdStr))
			return &cmd_note;
		if (cmdStr == "")
			return &cmd_null;

		return nullptr;
	}


	bool ScoreReader::CommandManager::isNumber(const std::basic_string<rch_type> &str) {
		if (str.empty())
			return false;

		for (auto ch : str) {
			if (ch < '0' || ch > '9')
				return false;
		}

		return true;
	}

}
