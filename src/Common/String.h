#pragma once
#include <ostream>

#include "Core/Core.h"

namespace mtk {
	
	class String
	{
	public:
		// Constructors/Destructors
		String();
		String(const String& _other);
		String(String&& _other) noexcept;
		explicit String(char _char);
		String(char* _char);
		String(const char* _char);
		String(const std::string& _string);
		String(std::string&& _string);
		String(std::string_view _string);
		String(const char* _begin, size_t _count);
		~String();

		// Accessors
		NODISCARD constexpr size_t Size() const { return m_Size; }
		NODISCARD constexpr const char* Data() const { return m_Data; }

		// Utility
		NODISCARD int32_t IndexOf(char _character) const;
		NODISCARD String Replace(char _oldChar, char _newChar) const;
		NODISCARD String Replace(const String& _oldString, char _newChar) const;
		NODISCARD String Replace(char _oldChar, const String& _newString) const;
		NODISCARD String Replace(const String& _oldString, const String& _newString) const;
		NODISCARD String Substring(size_t _start) const;
		NODISCARD String Substring(size_t _start, size_t _length) const;
		NODISCARD bool Equals(const String& _other) const;
		NODISCARD bool Equals(const std::string& _other) const;
		NODISCARD bool Equals(const char* _other) const;
		NODISCARD bool Equals(char _other) const;
		NODISCARD bool Contains(char _character) const;
		NODISCARD bool Contains(char* _string) const;
		NODISCARD bool Contains(const String& _string) const;
		NODISCARD bool Contains(const std::string& _string) const;
		NODISCARD bool StartsWith(char _character) const;
		NODISCARD bool StartsWith(const char* _string) const;
		NODISCARD bool StartsWith(const String& _string) const;
		NODISCARD bool StartsWith(const std::string& _string) const;
		NODISCARD bool EndsWith(char _character) const;
		NODISCARD bool EndsWith(const char* _string) const;
		NODISCARD bool EndsWith(const String& _string) const;
		NODISCARD bool EndsWith(const std::string& _string) const;
		NODISCARD String Trim(char _character) const;
		NODISCARD String Trim(const char* _characters) const;
		NODISCARD String TrimStart(char _character) const;
		NODISCARD String TrimStart(char* _characters) const;
		NODISCARD String TrimEnd(char _character) const;
		NODISCARD String TrimEnd(char* _characters) const;
		
		
		// Operator Overloads
		explicit operator std::string() const;
		
		char& operator[](const size_t& _index);
		const char& operator[](const size_t& _index) const;
		String& operator=(const String& _other);
		String& operator=(String&& _other) noexcept;
		String& operator=(char* _other);
		String& operator=(const char* _other);
		String& operator=(const std::string& _other);
		String& operator=(std::string&& _other);
		String& operator=(std::string_view _other);
		String& operator+=(const String& _other);
		String& operator+=(String&& _other);
		String& operator+=(char _other);
		String& operator+=(char* _other);
		String& operator+=(const char* _other);
		friend String operator+(const String& _left, const String& _right);
		friend String operator+(const String& _left, const char* _right);
		friend String operator+(const char* _left, const String& _right);
		friend bool operator==(const String& _left, const String& _right);
		friend bool operator!=(const String& _left, const String& _right);
		friend std::ostream& operator<<(std::ostream& _stream, const String& _current);

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	class BufferView
	{
	public:
		BufferView(const char* _ref);
		BufferView(const BufferView& _other) = default;
		BufferView(BufferView&&) = default;
		~BufferView() = default;

		NODISCARD size_t Size() const { return c_Size; }
		NODISCARD const char* Data() const { return c_StartRef; }

		NODISCARD BufferView Slice(size_t _start, size_t _end) const;
        
		NODISCARD const char& operator[](size_t _index);
		NODISCARD const char& operator[](size_t _index) const;
		BufferView& operator=(const BufferView& _other);
		BufferView& operator=(BufferView&& _other) noexcept;

	private:
		BufferView(const char* _startRef, const char* _endRef);
        
	private:
		const char* c_StartRef;
		const char* c_EndRef;
		size_t c_Size;
	};
}
