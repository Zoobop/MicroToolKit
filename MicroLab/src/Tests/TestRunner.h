#pragma once
#include <format>
#include <Windows.h>
#include <iostream>

template<typename _Type>
class TestRunner
{
};

#define TEST_ASSERT(condition)  if ((condition) == false) throw std::exception()

class TestLogger
{
public:
    enum struct TextColor : uint8_t
    {
        WHITE = 0,
        GREEN = 3,
        RED = 4,
        BLUE = 1,
    };
    
public:
    template<typename ... _Args>
    static void ShowMessage(const char* _context, _Args ... _args)
    {
        std::cout << std::vformat(_context, std::make_format_args(std::forward<_Args>(std::move(_args))...));
    }

    template<typename ... _Args>
    static void ShowTestRun(const char* _context, _Args ... _args)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)TextColor::WHITE);
        std::cout << "[TIME] " << std::vformat(_context, std::make_format_args(std::forward<_Args>(std::move(_args))...));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)TextColor::BLUE);
        std::cout << "[RUNNING]";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c_DefaultTextColor);
    }

    template<typename ... _Args>
    static void ShowResult(const char* _context, TextColor _color = TextColor::WHITE)
    {
        std::cout << "\b\b\b\b\b\b\b\b\b";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)_color);
        std::cout << _context << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)c_DefaultTextColor);
    }
    
private:
    static constexpr TextColor c_DefaultTextColor = TextColor::WHITE;
};

#define TEST_MESSAGE(message)                   ::TestLogger::ShowMessage(message)
#define TEST_MESSAGE_FORMAT(message, ...)       ::TestLogger::ShowMessage(message, __VA_ARGS__)
#define TEST_INFO(message)                      ::TestLogger::ShowTestRun(message)
#define TEST_INFO_FORMAT(message, ...)          ::TestLogger::ShowTestRun(message, __VA_ARGS__)
#define TEST_GOOD_RESULT(result)                ::TestLogger::ShowResult(result, ::TestLogger::TextColor::GREEN)
#define TEST_BAD_RESULT(result)                 ::TestLogger::ShowResult(result, ::TestLogger::TextColor::RED)