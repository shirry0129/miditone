//
//  Error.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/06.
//

#ifndef Error_hpp
#define Error_hpp


#include "Enum.hpp"


namespace score {


	template <class T>
	class Error : public Enum<T> {

		using char_type = char;
		using create_errmsg_func_t = std::function<std::basic_string<char_type>(T)>;

	public:
		Error(T value, const create_errmsg_func_t& _msgFunc = Enum<T>::defaultMsgFunc) noexcept
			: Enum<T>(value, _msgFunc) {}
		
		Error(const create_errmsg_func_t& _msgFunc = Enum<T>::defaultMsgFunc) noexcept
			: Enum<T>(static_cast<T>(0), _msgFunc) {}
		
		~Error() {}
		
		
		Error<T>& operator =(const T& _errVal) noexcept {
			this->assign(_errVal);
			return *this;
		}
		
		Error<T>& operator =(const int& _errVal) noexcept {
			this->assign(_errVal);
			return *this;
		}
	
		bool isError() const noexcept {
			return this->getCode() < 0;
		}
	};
	
}



#endif /* Error_hpp */
