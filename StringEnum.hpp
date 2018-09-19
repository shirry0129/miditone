/*

This class treats some strings as enum tag.

2018/9/14 written by Nanami Yamamoto

*/

#ifndef _STRINGITEM_HPP
#define _STRINGITEM_HPP


#include <map>

namespace utility {

	class StringEnum {
	public:

		template<class... Args>
		StringEnum(Args... args);

		~StringEnum();

		
		int getValue(const std::string &key);
		int insert(const std::string &key, int val);

	private:

		std::map<std::string, int> strEnum;


		static void insertEnum() {
			// end recursion
		}

		template <class Head, class... Tail>
		static void insertEnum(Head&& head, Tail&&... tail) {
			
			strEnum.insert(std::make_pair<head>, 0);
			// next
			insertEnum(std::forward<Tail>(tail)...);
		}


	};


	template<class... Args>
	inline StringEnum::StringEnum(Args... args) {
		// enter recursion
		insertEnum(args...);
	}

}


#endif // !_STRINGITEM_HPP
