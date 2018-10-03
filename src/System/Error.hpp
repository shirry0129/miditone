//
//  Error.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/02.
//

#ifndef Error_hpp
#define Error_hpp

#include <string>
#include <functional>


namespace score {

	template <class T>
	class Error {
	
		using char_type = char;
		using get_errormsg_func_t = std::function<std::basic_string<char_type>(T)>;
	
	public:
		Error(T value, const get_errormsg_func_t& _msgFunc = defaultMsgFunc) noexcept
			: msgFunc(_msgFunc), errorVal(value) {}
		
		Error(const get_errormsg_func_t& _msgFunc = defaultMsgFunc) noexcept
			: msgFunc(_msgFunc), errorVal(static_cast<T>(0)) {}
		
		
		virtual ~Error() {}
		
		void assign(T value) noexcept {
			errorVal = value;
		}
		
		T get() const noexcept {
			return errorVal;
		}
		
		bool isError() const noexcept {
			return getCode() < 0;
		}
		
		int getCode() const noexcept {
			return static_cast<int>(errorVal);
		}
		
		std::string getMessage() const noexcept {
			return msgFunc(errorVal);
		}
		
		Error& operator= (T _errVal) noexcept {
			assign(_errVal);
			return *this;
		}
		
		const get_errormsg_func_t &_getErrorMsgFunc() const noexcept {
			return msgFunc;
		}
		
	private:
		T errorVal;
		get_errormsg_func_t msgFunc;
		
		static std::basic_string<char_type> defaultMsgFunc(T val) {
			std::basic_string<char_type> msg;
			return msg;
		}
	};
	
	template<class T>
	bool operator==(const Error<T> &L, const Error<T> &R) {
		return L.get() == R.get();
	}
	
	template<class T>
	bool operator!=(const Error<T> &L, const Error<T> &R) {
		return !(L == R);
	}
	
	template<class T>
	bool operator==(const Error<T> &L, const T &R) {
		return L.get() == R;
	}
	
	template<class T>
	bool operator!=(const Error<T> &L, const T &R) {
		return !(L == R);
	}
	
	template<class T>
	bool operator==(const T &L, const Error<T> &R) {
		return L == R.get();
	}
	
	template<class T>
	bool operator!=(const T &L, const Error<T> &R) {
		return !(L == R);
	}


}



#endif /* Error_hpp */
