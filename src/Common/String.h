#pragma once
#include <ostream>

#include "List.h"
#include "Core/Core.h"

namespace mtk {

	class BufferView;
	
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
		String(BufferView _string);
		String(const char* _begin, size_t _count);
		String(const char* _begin, const char* _end);
		~String();

		// Accessors
		NODISCARD constexpr size_t Size() const { return m_Size; }
		NODISCARD constexpr const char* Data() const { return m_Data; }

		// Utility
		String& Append(const String& _string);
		String& Append(String&& _string) noexcept;
		String& Append(char _character);
		String& Append(char* _string);
		String& Append(const char* _string);
		String& Append(const std::string& _string);
		String& Append(std::string&& _string) noexcept;
		String& Append(std::string_view _string);
		String& Append(BufferView _string);
		NODISCARD void Insert(char _character, size_t _index) const;
		NODISCARD int32_t IndexOf(char _character) const;
		NODISCARD int32_t IndexOf(const String& _string, size_t _startIndex, size_t _length) const;
		NODISCARD int32_t LastIndexOf(char _character) const;
		NODISCARD int32_t LastIndexOf(const String& _string, size_t _startIndex, size_t _length) const;
		NODISCARD String Replace(char _oldChar, char _newChar) const;
		NODISCARD String Replace(const String& _oldString, char _newChar) const;
		NODISCARD String Replace(char _oldChar, const String& _newString) const;
		NODISCARD String Replace(const String& _oldString, const String& _newString) const;
		NODISCARD String Substring(size_t _start) const;
		NODISCARD String Substring(size_t _start, size_t _length) const;
		NODISCARD bool IsEmpty() const;
		NODISCARD bool Equals(const String& _other) const;
		NODISCARD bool Equals(const std::string& _other) const;
		NODISCARD bool Equals(const char* _other) const;
		NODISCARD bool Equals(char _other) const;
		NODISCARD bool Contains(char _character) const;
		NODISCARD bool Contains(const char* _string) const;
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
		NODISCARD String Trim(std::initializer_list<char>&& _characters) const;
		NODISCARD String TrimStart(char _character) const;
		NODISCARD String TrimStart(std::initializer_list<char>&& _characters) const;
		NODISCARD String TrimEnd(char _character) const;
		NODISCARD String TrimEnd(std::initializer_list<char>&& _characters) const;

		NODISCARD List<String> Split(char _delimiter = ' ') const;
		NODISCARD List<String> Split(std::initializer_list<char>&& _characters) const;
		
		// Operator Overloads
		explicit operator std::string() const;
		explicit operator BufferView() const;
		
		NODISCARD const char& operator[](const size_t& _index);
		NODISCARD const char& operator[](const size_t& _index) const;
		
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
		String& operator+=(const std::string& _other);
		String& operator+=(std::string&& _other);
		String& operator+=(std::string_view _other);
		String& operator+=(BufferView _other);
		friend String operator+(const String& _left, const String& _right);
		friend String operator+(const String& _left, const char* _right);
		friend String operator+(const char* _left, const String& _right);
		friend bool operator==(const String& _left, const String& _right);
		friend bool operator!=(const String& _left, const String& _right);
		friend std::ostream& operator<<(std::ostream& _stream, const String& _current);
		
	public:
		// Static
		static String Empty;
		
	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	class BufferView
	{
	public:
		friend String;
		
		BufferView(const char* _ref);
		BufferView(const String& _ref);
		BufferView(const BufferView&) = default;
		BufferView(BufferView&&) = default;
		~BufferView() = default;

		// Accessors
		NODISCARD size_t Size() const { return m_Size; }
		NODISCARD const char* Data() const { return c_StartRef; }

		// Utility
		NODISCARD BufferView Slice(size_t _start) const;
		NODISCARD BufferView Slice(size_t _start, size_t _length) const;
		NODISCARD bool IsEmpty() const;
		NODISCARD bool Equals(const BufferView& _other) const;

		// Operator Overloads
		NODISCARD const char& operator[](size_t _index);
		NODISCARD const char& operator[](size_t _index) const;
		
		BufferView& operator=(const BufferView& _other);
		BufferView& operator=(BufferView&& _other) noexcept;
		BufferView& operator=(const String& _other);
		friend bool operator==(const BufferView& _left, const BufferView& _right);
		friend bool operator!=(const BufferView& _left, const BufferView& _right);
		friend std::ostream& operator<<(std::ostream& _stream, const BufferView& _current);

	private:
		BufferView(const char* _startRef, const char* _endRef);
		BufferView();

	public:
		// Static
		static BufferView Empty;
		
	private:
		const char* c_StartRef;
		const char* c_EndRef;
		size_t m_Size;
	};
}
