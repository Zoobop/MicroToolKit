#pragma once
#include "Tests/TestRunner.h"

#include "Common/List.h"
#include "Example/Person.h"

using namespace mtk;

template<>
class TestRunner<List<ComplexType>>
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
    static bool Test_Add();
    static bool Test_Emplace();
    static bool Test_Insert();
    static bool Test_Remove();
    static bool Test_RemoveAt();
    static bool Test_Contains();
    static bool Test_Clear();
};
