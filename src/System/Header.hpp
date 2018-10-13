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
		
		
		const int&								id()		const noexcept;
		const std::basic_string<char_type>&		title()		const noexcept;
		const std::basic_string<char_type>&		artist()	const noexcept;
		const std::array<
		 	std::basic_string<char_type>, 3
		>&										level()		const noexcept;
		const Genre&							genre()		const noexcept;
		const std::vector<TempoEvent>&			tempo()		const noexcept;
		const std::vector<BeatEvent>&			beat()		const noexcept;
		
		
	private:
		
		int								m_id;
		std::basic_string<char_type>	m_title;
		std::basic_string<char_type>	m_artist;
		std::array<
			std::basic_string<char_type>,
			3
		>								m_level;
		Genre							m_genre;
		std::vector<TempoEvent>			m_tempo;
		std::vector<BeatEvent>			m_beat;
		
		scorereader_err_t prevError;
		
	};

}

#endif /* Header_hpp */
