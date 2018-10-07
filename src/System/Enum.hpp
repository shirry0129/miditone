//
//  Enum.hpp
//  MusicGame
//
//  Created by NanamiYamamoto on 2018/10/02.
//

#ifndef Enum_hpp
#define Enum_hpp

#include <string>
#include <functional>


template<class T>
class Enum;


template<class T>
bool operator==(const Enum<T>& L, const Enum<T>& R) {
	return L.get() == R.get();
}

template<class T>
bool operator==(const Enum<T>& L, const T& R) {
	return L.get() == R;
}

template<class T>
bool operator==(const T& L, const Enum<T>& R) {
	return L == R.get();
}

template<class T>
bool operator!=(const Enum<T>& L, const Enum<T>& R) {
	return !(L == R);
}

template<class T>
bool operator!=(const Enum<T>& L, const T& R) {
	return !(L == R);
}

template<class T>
bool operator!=(const T& L, const Enum<T>& R) {
	return !(L == R);
}

template<class T>
T operator|(const Enum<T>& L, const Enum<T>& R) {
	return L.getCode() | R.getCode();
}

template<class T>
T operator|(const Enum<T>& L, const T& R) {
	return L.getCode() | static_cast<int>(R);
}

template<class T>
T operator|(const T& L, const Enum<T>& R) {
	return static_cast<int>(L) | R.getCode();
}

template<class T>
T operator|(const Enum<T>& L, const int& R) {
	return L.getCode() | R;
}

template<class T>
T operator|(const int& L, const Enum<T>& R) {
	return L | R.getCode();
}


template<class T>
T operator&(const Enum<T>& L, const Enum<T>& R) {
	return L.getCode() & R.getCode();
}

template<class T>
T operator&(const Enum<T>& L, const T& R) {
	return L.getCode() & static_cast<int>(R);
}

template<class T>
T operator&(const T& L, const Enum<T>& R) {
	return static_cast<int>(L) | R.getCode();
}

template<class T>
T operator&(const Enum<T>& L, const int& R) {
	return L.getCode() & R;
}

template<class T>
T operator&(const int& L, const Enum<T>& R) {
	return L & R.getCode();
}




template <class T>
class Enum {
public:
	using char_type = char;
	using create_errmsg_func_t = std::function<std::basic_string<char_type>(T)>;

	Enum(T value, const create_errmsg_func_t& _msgFunc = defaultMsgFunc) noexcept
		: msgFunc(_msgFunc), enumVal(value) {}
	
	Enum(const create_errmsg_func_t& _msgFunc = defaultMsgFunc) noexcept
		: msgFunc(_msgFunc), enumVal(static_cast<T>(0)) {}

	
	virtual ~Enum() {}
	
	Enum<T>& assign(T value) noexcept {
		enumVal = value;
		return *this;
	}
	
	Enum<T>& assign(int value) noexcept {
		enumVal = static_cast<T>(value);
		return *this;
	}
	
	T get() const noexcept {
		return enumVal;
	}
	
	virtual int getCode() const noexcept {
		return static_cast<int>(enumVal);
	}
	
	virtual std::string getMessage() const noexcept {
		return msgFunc(enumVal);
	}
	
	Enum<T>& operator= (const T& _errVal) noexcept {
		assign(_errVal);
		return *this;
	}
	
	Enum<T>& operator= (const int& _errVal) noexcept {
		assign(_errVal);
		return *this;
	}
	
	const create_errmsg_func_t &_getEnumMsgFunc() const noexcept {
		return msgFunc;
	}
	
	
	
	Enum<T>& operator|=(const Enum<T>& R) {
		assign(this->getCode() | R.getCode());
		return *this;
	}
	
	Enum<T>& operator|=(const T& R) {
		assign(this->getCode() | R);
		return *this;
	}
	
	Enum<T>& operator|=(const int& R) {
		assign(this->getcode() | R);
		return *this;
	}
	
	Enum<T>& operator&=(const Enum<T>& R) {
		assign(this->getCode() & R.getCode());
		return *this;
	}
	
	Enum<T>& operator&=(const T& R) {
		assign(this->getCode() & R);
		return *this;
	}
	
	Enum<T>& operator&=(const int& R) {
		assign(this->getcode() & R);
		return *this;
	}
	
	static std::basic_string<char_type> defaultMsgFunc(T val) {
		std::basic_string<char_type> msg;
		return msg;
	}

protected:
	T enumVal;
	
	create_errmsg_func_t msgFunc;
	
};



#endif /* Enum_hpp */
