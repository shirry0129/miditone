//
//  ChEncoder.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/02.
//
//  Reference : <https://qiita.com/benikabocha/items/e943deb299d0f816f161>

#ifndef ChEncoder_hpp
#define ChEncoder_hpp

#include <string>
#include <array>


namespace ch_encoder {


	int __GetU8ByteCount(char ch);

	bool __IsU8LaterByte(char ch);

	bool __ConvChU8ToU32(const std::array<char, 4>& u8Ch, char32_t& u32Ch);
	
	bool UTF8toUTF32(const std::string& u8str, std::u32string& u32str);
	
	std::u32string toUTF32(const std::string& u8str);
	
	
	
	bool __ConvChU32ToU8(const char32_t u32Ch, std::array<char, 4>& u8Ch);


	bool UTF32toUTF8(const std::u32string& u32Str, std::string& u8Str);
	
	
	std::string toUTF8(const std::u32string u32str);

	
};


#endif /* ChEncoder_hpp */
