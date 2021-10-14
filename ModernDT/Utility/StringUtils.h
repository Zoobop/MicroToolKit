#pragma once
#include "Structures/List.h"

namespace mic {

	List<std::string_view> Split(const std::string& _string, char _delimiter = ' ')
	{
		List<std::string_view> stringData(_string.length());
		PROFILER();
		for (size_t i = 0, prev = 0; i < _string.length() + 1; i++) {
			if (_string[i] == _delimiter || _string[i] == '\0') {
				stringData.Emplace(_string.c_str() + prev, i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	std::string& ToUpper(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::toupper(_string[i]);
		}
		return _string;
	}

	std::string& ToLower(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::tolower(_string[i]);
		}
		return _string;
	}
}