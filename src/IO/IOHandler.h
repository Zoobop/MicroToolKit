#pragma once
#include <format>
#include <iostream>

#include "Common/String.h"

namespace mtk
{
    class IOHandler
    {
    public:
        IOHandler() = delete;
        
        static String ReadLine()
        {
            std::string dummy;
            std::getline(std::cin, dummy);
            return std::move(dummy);
        }

        static int8_t ReadKey()
        {
            int8_t key;
            std::cin >> key;
            return key;
        }
        
        template<typename _Type>
        static void WriteLine(const _Type& _object)
        {
            std::cout << _object << "\n";
        }
        
        template<typename ... _Args>
        static void WriteLine(const char* _content, _Args ... _args)
        {
            std::cout << std::vformat(_content, std::make_format_args(std::forward<_Args>(std::move(_args))...)) << "\n";
        }

        template<typename _Type>
        static void Write(const _Type& _object)
        {
            std::cout << _object;
        }
        
        template<typename ... _Args>
        static void Write(const char* _content, _Args ... _args)
        {
            std::cout << std::vformat(_content, std::make_format_args(std::forward<_Args>(std::move(_args))...));
        }
    };
}
