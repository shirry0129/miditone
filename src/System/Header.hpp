//
//  Header.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/13.
//

#ifndef Header_hpp
#define Header_hpp


#include "ScoreReader.hpp"


namespace score {

	using char_type = char32_t;
	constexpr size_t numofDifficulty = 3;

	enum class Genre {
		JPOP_ANIME = 0,
		GAME,
		NICONICO,
		VARIETY
	};
	

	class Header {
	public:
		
		Header();
		Header(const std::basic_string<char_type> filePath);
		
		~Header();
		
		scorereader_err_t read(const std::basic_string<char_type> filePath);
		
		scorereader_err_t getReaderError() const noexcept;
		
		
		int										id()		const noexcept;
		const std::basic_string<char_type>&		title()		const noexcept;
		const std::basic_string<char_type>&		artist()	const noexcept;
		const std::array<
		 	std::basic_string<char_type>, numofDifficulty
		>&										level()		const noexcept;
		const Genre&							genre()		const noexcept;
		const std::vector<TempoEvent>&			tempo()		const noexcept;
		const std::vector<BeatEvent>&			beat()		const noexcept;
		float 									chorusBegSec() const noexcept;
		float									chorusEndSec() const noexcept;
		
		
	private:
		
		int								m_id;
		std::basic_string<char_type>	m_title;
		std::basic_string<char_type>	m_artist;
		std::array<
			std::basic_string<char_type>,
			numofDifficulty
		>								m_level;
		Genre							m_genre;
		std::vector<TempoEvent>			m_tempo;
		std::vector<BeatEvent>			m_beat;
		float 							m_chorusBegSec;
		float							m_chorusEndSec;
		
		scorereader_err_t prevError;
		
	};

}

#endif /* Header_hpp */
