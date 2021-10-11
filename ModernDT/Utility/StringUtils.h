#pragma once
#include "Structures/List.h"

#define DEFAULT_CAPACITY	10

namespace mdt {

	List<std::string> Split(const std::string& _string, char _delimiter = ' ')
	{
		mdt::List<std::string> stringData(DEFAULT_CAPACITY);
		std::string member = "";
		for (size_t i = 0; i < _string.length() + 1; i++) {
			if (_string[i] == _delimiter || _string[i] == '\0') {
				stringData.Emplace(member);
				member = "";
			}
			else {
				member += _string[i];
			}
		}
		return stringData;
	}

	void ToUpper(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::toupper(_string[i]);
		}
	}

	void ToLower(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::tolower(_string[i]);
		}
	}

}