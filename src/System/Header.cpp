//
//  Header.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/13.
//

#include "Header.hpp"
#include "ChEncoder.hpp"

namespace score {

	Header::Header() {};

	Header::Header(const std::basic_string<char_type> filePath) {
			read(filePath);
	}

	Header::~Header() {};

	scorereader_err_t Header::read(const std::basic_string<char_type> filePath) {
		ScoreReader reader;
		
		if (reader.open(ch_encoder::toUTF8(filePath).c_str()).isError())
			return prevError = reader.getLastError();
		
		ScoreReader::Header h;
		if (reader.readHeader(h, "header").isError())
			return prevError = reader.getLastError();
		
		
		// update member
		m_id 		= h.id;
		m_title 	= ch_encoder::toUTF32(h.title);
		m_artist	= ch_encoder::toUTF32(h.artist);
		for (size_t i = 0; i < numofDifficulty; i++)
			m_level[i] = ch_encoder::toUTF32(h.level[i]);
		
		if (h.genre == "0")
			m_genre	= Genre::JPOP_ANIME;
		else if (h.genre == "1")
			m_genre	= Genre::GAME;
		else if (h.genre == "2")
			m_genre	= Genre::NICONICO;
		else if (h.genre == "3")
			m_genre	= Genre::VARIETY;
		
		for (const auto& t : h.tempo)
			m_tempo.emplace_back(t);
		
		for (const auto& b : h.beat)
			m_beat.emplace_back(b);
		
		
		return prevError = reader.getLastError();
	}
	
	scorereader_err_t Header::getReaderError() const noexcept {
		return prevError;
	}
	
	const int& Header::id() const noexcept {
		return m_id;
	}
	
	const std::basic_string<char_type>& Header::title() const noexcept {
		return m_title;
	}
	
	const std::basic_string<char_type>&	Header::artist() const noexcept {
		return m_artist;
	}
	
	const std::array<std::basic_string<char_type>, numofDifficulty>& Header::level() const noexcept {
		return m_level;
	}
	
	const Genre& Header::genre() const noexcept {
		return m_genre;
	}
	
	const std::vector<TempoEvent>& Header::tempo() const noexcept {
		return m_tempo;
	}
	
	const std::vector<BeatEvent>& Header::beat() const noexcept {
		return m_beat;
	}

}
