#pragma once
#include "TestRunner.h"
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
    static bool Test_Default_Constructor();
    static bool Test_Copy_Constructor();
    static bool Test_Move_Constructor();
    static bool Test_Explicit_Char_Constructor();
    static bool Test_Char_Pointer_Constructor();
    static bool Test_Const_Char_Pointer_Constructor();
    static bool Test_Std_String_Copy_Constructor();
    static bool Test_Std_String_Move_Constructor();
    static bool Test_Std_StringView_Constructor();
    static bool Test_Const_Char_Pointer_Range_Constructor();

    // Utility
    static bool Test_IndexOf();
    static bool Test_Replace_Char_Char();
    static bool Test_Replace_Char_String();
    static bool Test_Replace_String_Char();
    static bool Test_Replace_String_String();
    static bool Test_Substring_Start();
    static bool Test_Substring_Start_Length();
    static bool Test_Contains();
    static bool Test_StartsWith();
    static bool Test_EndsWith();
    static bool Test_Trim();
    static bool Test_TrimStart();
    static bool Test_TrimEnd();
};
