#pragma once
#include <string>

#include "Common/String.h"
#include "Common/List.h"

namespace mtk {

	template<typename _Type>
	_Type Parse(const String& _string)
	{
		return {};
	}

	template<>
	inline float Parse(const String& _string)
	{
		try
		{
			return std::stof((std::string)_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline double Parse(const String& _string)
	{
		try
		{
			return std::stod((std::string)_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline long double Parse(const String& _string)
	{
		try
		{
			return std::stold((std::string)_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline int8_t Parse(const String& _string)
	{
		try
		{
			return static_cast<int8_t>(std::stoi((std::string)_string));
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline int16_t Parse(const String& _string)
	{
		try
		{
			return static_cast<int16_t>(std::stoi((std::string)_string));
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline int32_t Parse(const String& _string)
	{
		try
		{
			return std::stoi((std::string)_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	template<>
	inline int64_t Parse(const String& _string)
	{
		try
		{
			return std::stoll((std::string)_string);
		}
		catch (const std::invalid_argument& e)
		{
			throw;
		}
	}

	inline bool TryParse(const String& _string, float& _out)
	{
		try
		{
			_out = std::stof((std::string)_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const String& _string, double& _out)
	{
		try
		{
			_out = std::stod((std::string)_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const String& _string, long double& _out)
	{
		try
		{
			_out = std::stold((std::string)_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const String& _string, int8_t& _out)
	{
		try
		{
			_out = static_cast<int8_t>(std::stoi((std::string)_string));
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const String& _string, int16_t& _out)
	{
		try
		{
			_out = static_cast<int16_t>(std::stoi((std::string)_string));
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline bool TryParse(const String& _string, int32_t& _out)
	{
		try
		{
			_out = std::stoi((std::string)_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}

	inline bool TryParse(const String& _string, int64_t& _out)
	{
		try
		{
			_out = std::stoll((std::string)_string);
			return true;
		}
		catch (const std::invalid_argument&)
		{
		}
		return false;
	}
	
	inline List<String> Split(const String& _string, char _delimiter = ' ')
	{
		List<String> stringData(_string.Size());
		for (size_t i = 0, prev = 0; i < _string.Size() + 1; i++) {
			if (_string[i] == '\r' || _string[i] == '\n') {
				if (i - prev == 0)
					prev++;
			}

			if (_string[i] == _delimiter || _string[i] == '\0') {
				stringData.Emplace(&_string[prev], i - prev);
				prev = i + 1;
			}
		}
		return stringData;
	}

	inline List<String> Split(const std::string_view& _string, char _delimiter = ' ')
	{
		List<String> stringData(_string.length());
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
			for (size_t i = 0; i < _left.length(); i++) {
				if (_left[i] != _right[i])
					return false;
			}
			return true;
		}
		return false;
	}

	inline String& ToUpper(String& _string)
	{
		for (size_t i = 0; i < _string.Size(); i++) {
			//_string[i] = (char) std::toupper(_string[i]);
		}
		return _string;
	}

	inline String& ToLower(String& _string)
	{
		for (size_t i = 0; i < _string.Size(); i++) {
			//_string[i] = (char) std::tolower(_string[i]);
		}
		return _string;
	}
	
}
