#pragma once
#include "Structures/List.h"

namespace mtk {

	template<typename _Type>
	_Type Parse(const std::string& _string)
	{
		return {};
	}

	template<>
	inline float Parse(const std::string& _string)
	{
		try
		{
			return std::stof(_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline double Parse(const std::string& _string)
	{
		try
		{
			return std::stod(_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline long double Parse(const std::string& _string)
	{
		try
		{
			return std::stold(_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline int8_t Parse(const std::string& _string)
	{
		try
		{
			return static_cast<int8_t>(std::stoi(_string));
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline int16_t Parse(const std::string& _string)
	{
		try
		{
			return static_cast<int16_t>(std::stoi(_string));
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline int32_t Parse(const std::string& _string)
	{
		try
		{
			return std::stoi(_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	template<>
	inline int64_t Parse(const std::string& _string)
	{
		try
		{
			return std::stoll(_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw e;
		}
	}

	inline bool TryParse(const std::string& _string, float& _out)
	{
		try
		{
			_out = std::stof(_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const std::string& _string, double& _out)
	{
		try
		{
			_out = std::stod(_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const std::string& _string, long double& _out)
	{
		try
		{
			_out = std::stold(_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const std::string& _string, int8_t& _out)
	{
		try
		{
			_out = static_cast<int8_t>(std::stoi(_string));
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const std::string& _string, int16_t& _out)
	{
		try
		{
			_out = static_cast<int16_t>(std::stoi(_string));
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const std::string& _string, int32_t& _out)
	{
		try
		{
			_out = std::stoi(_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const std::string& _string, int64_t& _out)
	{
		try
		{
			_out = std::stoll(_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline List<std::string_view> Split(const std::string& _string, char _delimiter = ' ')
	{
		List<std::string_view> stringData(_string.length());
		PROFILER();
		for (size_t i = 0, prev = 0; i < _string.length() + 1; i++) {
			if (_string[i] == '\r' || _string[i] == '\n') {
				if (i - prev == 0)
					prev++;
			}

			if (_string[i] == _delimiter || _string[i] == '\0') {
				stringData.Emplace(_string.c_str() + prev, i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	inline List<std::string_view> Split(const std::string_view& _string, char _delimiter = ' ')
	{
		List<std::string_view> stringData(_string.length());
		PROFILER();
		for (size_t i = 0, prev = 0; i < _string.length(); i++) {
			if (_string[i] == '\r' || _string[i] == '\n') {
				if (i - prev == 0)
					prev++;
			}

			if (_string[i] == _delimiter || _string[i] == '\0') {
				stringData.Emplace(_string.data() + prev, i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	inline bool IsIdentical(const std::string& _left, const std::string& _right)
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

	inline std::string& ToUpper(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::toupper(_string[i]);
		}
		return _string;
	}

	inline std::string& ToLower(std::string& _string)
	{
		for (auto i = 0; i < _string.length(); i++) {
			_string[i] = std::tolower(_string[i]);
		}
		return _string;
	}
	
}
