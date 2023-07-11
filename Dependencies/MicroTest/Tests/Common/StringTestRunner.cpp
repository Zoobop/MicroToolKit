#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Common/String.hpp"

using namespace Micro;

namespace Micro::Tests
{
	TEST_CLASS(StringTestRunner)
	{
	public:
		// Setup
		TEST_CLASS_INITIALIZE(Initialization)
		{
			new(&TestString) String("Zoobop is a god");
		}

		// Unit Tests
		TEST_METHOD(Test_Default_Constructor_Initializes_As_Null)
		{
			String string;

			Assert::IsTrue(string.Data() == nullptr);
			Assert::IsTrue(string.Length() == 0);
		}

		TEST_METHOD(Test_Copy_Constructor_Copies)
		{
			String string(TestString);

			Assert::IsTrue(string == TestString);
			Assert::IsTrue(string.Length() == TestString.Length());
		}

		TEST_METHOD(Test_Move_Constructor_Moves)
		{
			String moveList = TestString;
			const auto data = moveList.Data();
			const auto length = moveList.Length();
			String string(std::move(moveList));

			Assert::IsTrue(string.Data() == data);
			Assert::IsTrue(string.Length() == length);
		}

		TEST_METHOD(Test_Null_Constructor_Acts_As_Default)
		{
			String string = nullptr;

			Assert::IsTrue(string.Data() == nullptr);
			Assert::IsTrue(string.Length() == 0);
		}

	private:
		inline static String TestString;
	};
}
