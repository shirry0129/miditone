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


	int __GetU8ByteCount(char ch) {
		if (0 <= uint8_t(ch) && uint8_t(ch) < 0x80) {
			return 1;
		}
		if (0xC2 <= uint8_t(ch) && uint8_t(ch) < 0xE0) {
			return 2;
		}
		if (0xE0 <= uint8_t(ch) && uint8_t(ch) < 0xF0) {
			return 3;
		}
		if (0xF0 <= uint8_t(ch) && uint8_t(ch) < 0xF8) {
			return 4;
		}
		return 0;
	}

	bool __IsU8LaterByte(char ch) {
		return 0x80 <= uint8_t(ch) && uint8_t(ch) < 0xC0;
	}

	bool __ConvChU8ToU32(const std::array<char, 4>& u8Ch, char32_t& u32Ch) {
		int numBytes = __GetU8ByteCount(u8Ch[0]);
		if (numBytes == 0) {
			return false;
		}
		switch (numBytes) {
			case 1:
				u32Ch = char32_t(uint8_t(u8Ch[0]));
				break;
			case 2:
				if (!__IsU8LaterByte(u8Ch[1])) {
					return false;
				}
				if ((uint8_t(u8Ch[0]) & 0x1E) == 0) {
					return false;
				}

				u32Ch = char32_t(u8Ch[0] & 0x1F) << 6;
				u32Ch |= char32_t(u8Ch[1] & 0x3F);
				break;
			case 3:
				if (!__IsU8LaterByte(u8Ch[1]) || !__IsU8LaterByte(u8Ch[2])) {
					return false;
				}
				if ((uint8_t(u8Ch[0]) & 0x0F) == 0 &&
					(uint8_t(u8Ch[1]) & 0x20) == 0) {
					return false;
				}

				u32Ch = char32_t(u8Ch[0] & 0x0F) << 12;
				u32Ch |= char32_t(u8Ch[1] & 0x3F) << 6;
				u32Ch |= char32_t(u8Ch[2] & 0x3F);
				break;
			case 4:
				if (!__IsU8LaterByte(u8Ch[1]) || !__IsU8LaterByte(u8Ch[2]) ||
					!__IsU8LaterByte(u8Ch[3])) {
					return false;
				}
				if ((uint8_t(u8Ch[0]) & 0x07) == 0 &&
					(uint8_t(u8Ch[1]) & 0x30) == 0) {
					return false;
				}

				u32Ch = char32_t(u8Ch[0] & 0x07) << 18;
				u32Ch |= char32_t(u8Ch[1] & 0x3F) << 12;
				u32Ch |= char32_t(u8Ch[2] & 0x3F) << 6;
				u32Ch |= char32_t(u8Ch[3] & 0x3F);
				break;
			default:
				return false;
		}

		return true;
	}
	
	bool UTF8toUTF32(const std::string& u8str, std::u32string& u32str) {
		for (auto u8It = u8str.begin(); u8It != u8str.end(); ++u8It) {
			auto numBytes = __GetU8ByteCount((*u8It));
			if (numBytes == 0) {
				return false;
			}

			std::array<char, 4> u8Ch;
			u8Ch[0] = (*u8It);
			for (int i = 1; i < numBytes; i++) {
				++u8It;
				if (u8It == u8str.end()) {
					return false;
				}
				u8Ch[i] = (*u8It);
			}

			char32_t u32Ch;
			if (!__ConvChU8ToU32(u8Ch, u32Ch)) {
				return false;
			}

			u32str.push_back(u32Ch);
		}
		return true;
	}
	
	std::u32string toUTF32(const std::string& u8str) {
		std::u32string u32str;
		UTF8toUTF32(u8str, u32str);
		return u32str;
	}

	
};


#endif /* ChEncoder_hpp */
