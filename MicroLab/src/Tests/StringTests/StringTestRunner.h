#pragma once
#include "Tests/TestRunner.h"

#include "Common/String.h"
#include "Core/Function.h"

using namespace mtk;

template<>
class TestRunner<String>
{
public:
    TestRunner() = delete;

    static void Run();

private:
    /* ======= Utility ======= */

    static void ShowResults(const Func<bool>& _function, const String& _testName);
    
private:
    /* ======== Tests ======== */

    // Constructors
    static bool Test_Constructors();

    // Utility
    static bool Test_IndexOf_Char();
    static bool Test_IndexOf_String();
    static bool Test_LastIndexOf_Char();
    static bool Test_LastIndexOf_String();
    static bool Test_Replace_Char_Char();
    static bool Test_Replace_Char_String();
    static bool Test_Replace_String_Char();
    static bool Test_Replace_String_String();
    static bool Test_Substring_Start();
    static bool Test_Substring_Start_Length();
    static bool Test_Contains_Char();
    static bool Test_Contains_Const_Char_Pointer();
    static bool Test_Contains_String();
    static bool Test_Contains_Std_String();
    static bool Test_Contains_Std_String_View();
    static bool Test_StartsWith();
    static bool Test_EndsWith();
    static bool Test_Trim();
    static bool Test_TrimStart();
    static bool Test_TrimEnd();
};