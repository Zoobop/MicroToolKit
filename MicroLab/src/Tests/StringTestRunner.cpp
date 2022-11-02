#include "StringTestRunner.h"

#define TEST(func) TestRunner<String>::ShowResults(func, STRINGIFY(func))

void TestRunner<String>::Run()
{
    TEST_MESSAGE       ("======================================================================\n");
    TEST_MESSAGE_FORMAT("  Zoobop's {} TestRunner                                              \n", STRINGIFY(String));
    TEST_MESSAGE       ("======================================================================\n");
    
    TEST(Test_Default_Constructor);
    TEST(Test_Copy_Constructor);
    TEST(Test_Move_Constructor);
    TEST(Test_Explicit_Char_Constructor);
    TEST(Test_Char_Pointer_Constructor);
    TEST(Test_Const_Char_Pointer_Constructor);
    TEST(Test_Std_String_Copy_Constructor);
    TEST(Test_Std_String_Move_Constructor);
    TEST(Test_Std_StringView_Constructor);
    TEST(Test_Const_Char_Pointer_Range_Constructor);

    TEST(Test_IndexOf);
    TEST(Test_Replace_Char_Char);
    TEST(Test_Replace_Char_String);
    TEST(Test_Replace_String_Char);
    TEST(Test_Replace_String_String);
    TEST(Test_Substring_Start);
    TEST(Test_Substring_Start_Length);
}

void TestRunner<String>::ShowResults(const Func<bool>& _function, const String& _testName)
{
    TEST_INFO_FORMAT("Running Test ({})... ", _testName.Data());
    if (_function())
    {
        TEST_GOOD_RESULT("[SUCCESS]");
        return;
    }
    
    TEST_BAD_RESULT("[FAILURE]");
}

bool TestRunner<String>::Test_Default_Constructor()
{
    try
    {
        // Setup
        const String empty;

        // Assert
        TEST_ASSERT(empty == String());
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Copy_Constructor()
{
    try
    {
        // Setup
        const String string1 = "Zoobop is a good guy";
        const String string2 = string1;

        // Assert
        TEST_ASSERT(string1.Equals("Zoobop is a good guy"));
        TEST_ASSERT(string2.Equals("Zoobop is a good guy"));
        TEST_ASSERT(string1.Equals(string2));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Move_Constructor()
{
    try
    {
        // Setup
        String string1 = "Zoobop is a good guy";
        String string2 = std::move(string1);

        // Assert
        TEST_ASSERT(!string1.Equals("Zoobop is a good guy"));
        TEST_ASSERT(string2.Equals("Zoobop is a good guy"));
        TEST_ASSERT(!string1.Equals(string2));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Explicit_Char_Constructor()
{
    try
    {
        // Setup
        const String character('a');

        // Assert
        TEST_ASSERT(character.Equals('a'));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Char_Pointer_Constructor()
{
    try
    {
        // Setup
        char str[6];
        str[0] = 'a';
        str[1] = 'b';
        str[2] = 'c';
        str[3] = 'd';
        str[4] = 'e';
        str[5] = 0;
        const String string = str;

        // Assert
        TEST_ASSERT(string.Size() == 5); // 1 + length of str
        TEST_ASSERT(string.Equals("abcde"));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Const_Char_Pointer_Constructor()
{
    try
    {
        // Setup
        const char* str = "Zoobop is a good guy";
        const String string = str;

        // Assert
        TEST_ASSERT(string.Equals("Zoobop is a good guy"));
        TEST_ASSERT(string.Equals(str));
        TEST_ASSERT(string.Size() == 20);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Std_String_Copy_Constructor()
{
    try
    {
        // Setup
        const std::string stdString = "Zoobop is a good guy";
        String string = stdString;

        // Assert
        TEST_ASSERT(string.Equals(stdString));
        TEST_ASSERT(string.Size() == stdString.size());
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Std_String_Move_Constructor()
{
    try
    {
        std::string stdString = "Zoobop is a good guy";
        String string = std::move(stdString);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Std_StringView_Constructor()
{
    try
    {
        std::string stdString = "Zoobop is a good guy";
        std::string_view strview = stdString;
        String string = strview;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Const_Char_Pointer_Range_Constructor()
{
    try
    {
        String string = { "Zoobop is a good guy", 10 };
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}



bool TestRunner<String>::Test_IndexOf()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid character
        const int validIndex = testString.IndexOf('Z');

        // Invalid character
        const int invalidIndex = testString.IndexOf('e');

        // Assert
        TEST_ASSERT(validIndex != -1);
        TEST_ASSERT(invalidIndex == -1);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Replace_Char_Char()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Change all 'o' to 'e' (char, char)
        const String goodString = testString.Replace('o', 'e');

        // Change all 'r' to 'e'
        const String badString = testString.Replace('r', 'e');

        // Assert
        TEST_ASSERT(goodString.Equals("Zeebep is a geed guy"));
        TEST_ASSERT(badString.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Replace_Char_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Change all 'o' to "12" (char, string)
        const String goodString = testString.Replace('o', "12");

        // Change all 'r' to "12"
        const String badString = testString.Replace('r', "12");

        // Assert
        TEST_ASSERT(goodString.Equals("Z1212b12p is a g1212d guy"));
        TEST_ASSERT(badString.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Replace_String_Char()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Change all "oo" to 'x' (string, char)
        const String goodString = testString.Replace("oo", 'x');

        // Change all "bb" to "12"
        const String badString = testString.Replace("bb", 'x');

        // Assert
        TEST_ASSERT(goodString.Equals("Zxbop is a gxd guy"));
        TEST_ASSERT(badString.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Replace_String_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Change all "oo" to "" (string, string)
        const String goodString = testString.Replace("oo", "");

        // Change all "bb" to ""
        const String badString = testString.Replace("bb", "");

        // Assert
        TEST_ASSERT(goodString.Equals("Zbop is a gd guy"));
        TEST_ASSERT(badString.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Substring_Start()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Start substring at index 0
        const String goodString1 = testString.Substring(0);

        // Start substring at index 10
        const String goodString2 = testString.Substring(10);

        // Start substring at index 20
        const String badString1 = testString.Substring(20);
        
        // Start substring at index -1
        const String badString2 = testString.Substring(-1);

        // Assert
        TEST_ASSERT(goodString1.Equals(testString));
        TEST_ASSERT(goodString2.Equals(" a good guy"));
        TEST_ASSERT(badString1.Equals(String{ })); // empty string
        TEST_ASSERT(badString2.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Substring_Start_Length()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Bounds check (0, 20)
        const String goodString1 = testString.Substring(0, testString.Size());

        // Normal substring
        const String goodString2 = testString.Substring(10, 14);

        // Same index (x, x)
        const String badString1 = testString.Substring(20, 20);
        
        // Start larger than end
        const String badString2 = testString.Substring(5, 3);

        // Assert
        TEST_ASSERT(goodString1.Equals(testString));
        TEST_ASSERT(goodString2.Equals(" a good guy"));
        TEST_ASSERT(badString1.Equals(String{ })); // empty string
        TEST_ASSERT(badString2.Equals(testString));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_StartsWith()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_EndsWith()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Trim()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_TrimStart()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_TrimEnd()
{
    try
    {
        String empty;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
