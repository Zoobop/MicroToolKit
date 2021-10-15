#pragma once
#include "Structures/List.h"

namespace mtk {

	List<std::string_view> Split(const std::string& _string, char _delimiter = ' ')
	{
		List<std::string_view> stringData(_string.length());
		PROFILER();
		for (size_t i = 0, prev = 0; i < _string.length() + 1; i++) {
			if (_string[i] == _delimiter || _string[i] == '\0' || _string[i] == '\r' || _string[i] == std::string::npos) {
				stringData.Emplace(_string.c_str() + prev, i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	List<std::string_view> Split(const std::string_view& _string, char _delimiter = ' ')
	{
		List<std::string_view> stringData(_string.length());
		PROFILER();
		for (size_t i = 0, prev = 0; i < _string.length() + 1; i++) {
			if (_string[i] == _delimiter || _string[i] == '\0' || _string[i] == '\r' || _string[i] == std::string::npos) {
				stringData.Emplace(_string.data() + prev, i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	bool IsIdentical(const std::string& _left, const std::string& _right)
	{
		if (_left.length() == _right.length()) {
			for (auto i = 0; i < _left.length(); i++) {
				if (_left[i] != _right[i])
					return false;
			}
			return true;
		}
		return false;
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