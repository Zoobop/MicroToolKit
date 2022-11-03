#include "StringTestRunner.h"

#define TEST(func) TestRunner<String>::ShowResults(func, NAMEOF(func))

void TestRunner<String>::Run()
{
    TEST_MESSAGE       ("======================================================================\n");
    TEST_MESSAGE_FORMAT("  Zoobop's {} TestRunner                                              \n", NAMEOF(String));
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

    TEST(Test_IndexOf_Char);
    TEST(Test_IndexOf_String);
    TEST(Test_LastIndexOf_Char);
    TEST(Test_LastIndexOf_String);
    TEST(Test_Replace_Char_Char);
    TEST(Test_Replace_Char_String);
    TEST(Test_Replace_String_Char);
    TEST(Test_Replace_String_String);
    TEST(Test_Substring_Start);
    TEST(Test_Substring_Start_Length);
    TEST(Test_Contains_Char);
    TEST(Test_Contains_Const_Char_Pointer);
    TEST(Test_Contains_String);
    TEST(Test_Contains_Std_String);
    TEST(Test_Contains_Std_String_View);
    TEST(Test_StartsWith);
    TEST(Test_EndsWith);
    TEST(Test_Trim);
    TEST(Test_TrimStart);
    TEST(Test_TrimEnd);
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
        TEST_ASSERT(empty == String::Empty);
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



bool TestRunner<String>::Test_IndexOf_Char()
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
        TEST_ASSERT(validIndex == 0);
        TEST_ASSERT(invalidIndex == -1);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_IndexOf_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid string, Valid index, Valid length
        const int validIndex = testString.IndexOf("Zoobop", 0, 10);

        // Valid string, Valid index, Invalid length
        const int invalidIndex1 = testString.IndexOf("Zoobop", 0, 0);
        const int invalidIndex2 = testString.IndexOf("Zoobop", 0, 50);

        // Valid string, Invalid index, Valid length
        const int invalidIndex3 = testString.IndexOf("Zoobop", -1, 10);
        const int invalidIndex4 = testString.IndexOf("Zoobop", 50, 10);

        // Invalid string, Valid index, Valid length
        const int invalidIndex5 = testString.IndexOf("bad", 0, 10);
        const int invalidIndex6 = testString.IndexOf("", 0, 10);

        // Assert
        TEST_ASSERT(validIndex == 0);
        TEST_ASSERT(invalidIndex1 == -1);
        TEST_ASSERT(invalidIndex2 == -1);
        TEST_ASSERT(invalidIndex3 == -1);
        TEST_ASSERT(invalidIndex4 == -1);
        TEST_ASSERT(invalidIndex5 == -1);
        TEST_ASSERT(invalidIndex6 == -1);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_LastIndexOf_Char()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid character
        const int validIndex = testString.LastIndexOf('g');

        // Invalid character
        const int invalidIndex = testString.LastIndexOf('e');

        // Assert
        TEST_ASSERT(validIndex == 17);
        TEST_ASSERT(invalidIndex == -1);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_LastIndexOf_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid string, Valid index, Valid length
        const int validIndex1 = testString.LastIndexOf("oo", 0, 10);
        const int validIndex2 = testString.LastIndexOf("oo", 0, testString.Size());

        // Valid string, Valid index, Invalid length
        const int invalidIndex1 = testString.LastIndexOf("oo", 0, 0);
        const int invalidIndex2 = testString.LastIndexOf("oo", 0, 50);

        // Valid string, Invalid index, Valid length
        const int invalidIndex3 = testString.LastIndexOf("oo", -1, 10);
        const int invalidIndex4 = testString.LastIndexOf("oo", 50, 10);

        // Invalid string, Valid index, Valid length
        const int invalidIndex5 = testString.LastIndexOf("bad", 0, 10);
        const int invalidIndex6 = testString.LastIndexOf("", 0, 10);

        // Assert
        TEST_ASSERT(validIndex1 == 1);
        TEST_ASSERT(validIndex2 == 13);
        TEST_ASSERT(invalidIndex1 == -1);
        TEST_ASSERT(invalidIndex2 == -1);
        TEST_ASSERT(invalidIndex3 == -1);
        TEST_ASSERT(invalidIndex4 == -1);
        TEST_ASSERT(invalidIndex5 == -1);
        TEST_ASSERT(invalidIndex6 == -1);
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
        TEST_ASSERT(goodString2.Equals("a good guy"));
        TEST_ASSERT(badString1.Equals(String::Empty));
        TEST_ASSERT(badString2.Equals(String::Empty));
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
        
        // Valid start, Valid length
        const String validString = testString.Substring(0, testString.Size());

        // Valid start, Invalid length
        const String invalidString1 = testString.Substring(10, -1);

        // Invalid start, Valid length
        const String invalidString2 = testString.Substring(-1, testString.Size());

        // Assert
        TEST_ASSERT(validString.Equals(testString));
        TEST_ASSERT(invalidString1.Equals(String::Empty));
        TEST_ASSERT(invalidString2.Equals(String::Empty));
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains_Char()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid contains
        const bool validContains = testString.Contains('Z');

        // Invalid contains
        const bool invalidContains = testString.Contains('x');
        
        // Assert
        TEST_ASSERT(validContains == true);
        TEST_ASSERT(invalidContains == false);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains_Const_Char_Pointer()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid contains
        const bool validContains1 = testString.Contains("Z");
        const bool validContains2 = testString.Contains("Zoo");
        const bool validContains3 = testString.Contains(" ");

        // Invalid contains
        const bool invalidContains1 = testString.Contains("x");
        const bool invalidContains2 = testString.Contains("xxx");
        const bool invalidContains3 = testString.Contains("");
        
        // Assert
        TEST_ASSERT(validContains1 == true);
        TEST_ASSERT(validContains2 == true);
        TEST_ASSERT(validContains3 == true);
        TEST_ASSERT(invalidContains1 == false);
        TEST_ASSERT(invalidContains2 == false);
        TEST_ASSERT(invalidContains3 == false);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid contains
        const bool validContains1 = testString.Contains(String { "Z" });
        const bool validContains2 = testString.Contains(String { "Zoo" });
        const bool validContains3 = testString.Contains(String { " " });

        // Invalid contains
        const bool invalidContains1 = testString.Contains(String { "x" });
        const bool invalidContains2 = testString.Contains(String { "xxx" });
        const bool invalidContains3 = testString.Contains(String { "" });
        
        // Assert
        TEST_ASSERT(validContains1 == true);
        TEST_ASSERT(validContains2 == true);
        TEST_ASSERT(validContains3 == true);
        TEST_ASSERT(invalidContains1 == false);
        TEST_ASSERT(invalidContains2 == false);
        TEST_ASSERT(invalidContains3 == false);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains_Std_String()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid contains
        const bool validContains1 = testString.Contains(std::string { "Z" });
        const bool validContains2 = testString.Contains(std::string { "Zoo" });
        const bool validContains3 = testString.Contains(std::string { " " });

        // Invalid contains
        const bool invalidContains1 = testString.Contains(std::string { "x" });
        const bool invalidContains2 = testString.Contains(std::string { "xxx" });
        const bool invalidContains3 = testString.Contains(std::string { "" });
        
        // Assert
        TEST_ASSERT(validContains1 == true);
        TEST_ASSERT(validContains2 == true);
        TEST_ASSERT(validContains3 == true);
        TEST_ASSERT(invalidContains1 == false);
        TEST_ASSERT(invalidContains2 == false);
        TEST_ASSERT(invalidContains3 == false);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool TestRunner<String>::Test_Contains_Std_String_View()
{
    try
    {
        // Setup
        const String testString = "Zoobop is a good guy";
        
        // Valid contains
        const bool validContains1 = testString.Contains(std::string_view { "Z" });
        const bool validContains2 = testString.Contains(std::string_view { "Zoo" });
        const bool validContains3 = testString.Contains(std::string_view { " " });

        // Invalid contains
        const bool invalidContains1 = testString.Contains(std::string_view { "x" });
        const bool invalidContains2 = testString.Contains(std::string_view { "xxx" });
        const bool invalidContains3 = testString.Contains(std::string_view { "" });
        
        // Assert
        TEST_ASSERT(validContains1 == true);
        TEST_ASSERT(validContains2 == true);
        TEST_ASSERT(validContains3 == true);
        TEST_ASSERT(invalidContains1 == false);
        TEST_ASSERT(invalidContains2 == false);
        TEST_ASSERT(invalidContains3 == false);
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
        // Setup
        const String testString = "111101111";
        
        // Valid trims
        const String validTrim = testString.Trim('1');

        // Invalid trims
        const String invalidTrim = testString.Trim('x');
        
        // Assert
        TEST_ASSERT(validTrim.Equals("0"));
        TEST_ASSERT(invalidTrim.Equals("111101111"));
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
