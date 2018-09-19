#include "StringEnum.hpp"


namespace utility {

	StringEnum::~StringEnum() {}

	int StringEnum::getValue(const std::string & key) {
		return strEnum.at(key);
	}

	int StringEnum::insert(const std::string & key, int val) {
		return 0;
	}


}