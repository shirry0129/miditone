﻿//
//  Header.cpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/13.
//

#include "Header.hpp"
#include "ChEncoder.hpp"

namespace score {

	Header::Header() {};

	Header::Header(const boost::filesystem::path& filePath) {
		read(filePath);
	}

	Header::~Header() {};

	scorereader_err_t Header::read(const boost::filesystem::path& filePath) {
		ScoreReader reader;

		if (reader.open(filePath).isError())
			return prevError = reader.getLastError();

		ScoreReader::Header h;
		if (reader.readHeader(h, "header").isError())
			return prevError = reader.getLastError();

#ifdef MIDITONE_WIIBALANCEBOARD
        ScoreReader::Header wii_h;
        if (reader.readHeader(wii_h, "header-wii").isError())
            return prevError = reader.getLastError();
#endif

		// update member
		m_id = h.id;
		m_title = ch_encoder::toUTF32(h.title);
		m_artist = ch_encoder::toUTF32(h.artist);

#ifdef MIDITONE_WIIBALANCEBOARD
        for (size_t i = 0; i < numofDifficulty; i++)
            m_level[i] = ch_encoder::toUTF32(wii_h.level[i]);
#else
        for (size_t i = 0; i < numofDifficulty; i++)
            m_level[i] = ch_encoder::toUTF32(h.level[i]);
#endif

		if (h.genre == "0")
			m_genre = Genre::JPOP_ANIME;
		else if (h.genre == "1")
			m_genre = Genre::GAME;
		else if (h.genre == "2")
			m_genre = Genre::NICONICO;
		else if (h.genre == "3")
			m_genre = Genre::VARIETY;

		for (const auto& t : h.tempo)
			m_tempo.emplace_back(t);

		for (const auto& b : h.beat)
			m_beat.emplace_back(b);

		m_chorusBegSec = h.chorusBegSec;
		m_chorusEndSec = h.chorusEndSec;


		return prevError = reader.getLastError();
	}

	scorereader_err_t Header::getReaderError() const noexcept {
		return prevError;
	}

	int Header::id() const noexcept {
		return m_id;
	}

	const std::basic_string<char_type>& Header::title() const noexcept {
		return m_title;
	}

	const std::basic_string<char_type>& Header::artist() const noexcept {
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
	
	float Header::chorusBegSec() const noexcept {
		return m_chorusBegSec;
	}
	
	float Header::chorusEndSec() const noexcept {
		return m_chorusEndSec;
	}


}
