#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Collections/LinkedList.hpp"

using namespace Micro;

namespace Micro::Tests
{
	TEST_CLASS(LinkedListTestRunner_Primitive)
	{
	public:
		// Aliases
		using Set = LinkedList<int>;

		// Setup
		TEST_CLASS_INITIALIZE(Initialization)
		{
			new(&TestList) Set(0, 1, 2, 3, 4);
		}

		// Unit Tests
		TEST_METHOD(Test_Default_Constructor_Initializes_To_Empty_List)
		{
			Set linkedList;

			Assert::IsTrue(linkedList.Size() == 0);
			Assert::IsTrue(linkedList.Head() == nullptr);
			Assert::IsTrue(linkedList.Tail() == nullptr);
		}

		TEST_METHOD(Test_Copy_Constructor_Copies)
		{
			Set linkedList(TestList);

			Assert::IsTrue(linkedList.Size() == TestList.Size());
			Assert::IsTrue(linkedList.Head()->Value == TestList.Head()->Value);
			Assert::IsTrue(linkedList.Tail()->Value == TestList.Tail()->Value);
		}

		TEST_METHOD(Test_Move_Constructor_Moves)
		{
			Set moveList = TestList;
			const auto size = moveList.Size();
			const auto headPtr = moveList.Head();
			const auto tailPtr = moveList.Tail();
			Set linkedList(std::move(moveList));

			Assert::IsTrue(linkedList.Size() == size);
			Assert::IsTrue(linkedList.Head() == headPtr);
			Assert::IsTrue(linkedList.Tail() == tailPtr);
		}

		TEST_METHOD(Test_Initializer_List_Constructor_To_Node_Chain)
		{
			constexpr auto size = 5;
			Set linkedList({0, 1, 2, 3, 4});

			Assert::IsTrue(linkedList.Size() == size);
			Assert::IsTrue(linkedList.Head()->Value == 0);
			Assert::IsTrue(linkedList.Tail()->Value == 4);
		}

		TEST_METHOD(Test_Param_Pack_Constructor_To_Node_Chain)
		{
			constexpr auto size = 5;
			Set linkedList(0, 1, 2, 3, 4);

			Assert::IsTrue(linkedList.Size() == size);
			Assert::IsTrue(linkedList.Head()->Value == 0);
			Assert::IsTrue(linkedList.Tail()->Value == 4);
		}

		TEST_METHOD(Test_PushBack_First_Element)
		{
			Set linkedList;

			linkedList.PushBack(0);
			Assert::IsTrue(linkedList.Size() == 1);
			Assert::IsTrue(linkedList.Head() == linkedList.Tail());
		}

		TEST_METHOD(Test_PushBack_Existing_Elements)
		{
			Set linkedList(0, 1, 2);

			linkedList.PushBack(3);
			Assert::IsTrue(linkedList.Size() == 4);
			Assert::IsTrue(linkedList.Head()->Value == 0);
			Assert::IsTrue(linkedList.Tail()->Value == 3);
		}

		TEST_METHOD(Test_PushFront_First_Element)
		{
			Set linkedList;

			linkedList.PushFront(0);
			Assert::IsTrue(linkedList.Size() == 1);
			Assert::IsTrue(linkedList.Head() == linkedList.Tail());
		}

		TEST_METHOD(Test_PushFront_Existing_Elements)
		{
			Set linkedList(0, 1, 2);

			linkedList.PushFront(3);
			Assert::IsTrue(linkedList.Size() == 4);
			Assert::IsTrue(linkedList.Head()->Value == 3);
			Assert::IsTrue(linkedList.Tail()->Value == 2);
		}

		TEST_METHOD(Test_At_With_Invalid_Index)
		{
			try
			{
				Set linkedList(0, 1, 2, 3);
				const auto _ = linkedList.At(4);

				Assert::Fail();
			}
			catch (const IndexOutOfRangeException&)
			{
			}
		}

		TEST_METHOD(Test_At_With_Valid_Index)
		{
			Set linkedList(0, 1, 2, 3);

			const auto elem = linkedList.At(3);
			Assert::IsTrue(elem == 3);
		}

		TEST_METHOD(Test_PopBack_With_No_Elements)
		{
			try
			{
				Set linkedList;
				const auto _ = linkedList.PopBack();

				Assert::Fail();
			}
			catch (const InvalidOperationException&)
			{
			}
		}

		TEST_METHOD(Test_PopBack_With_One_Element)
		{
			Set linkedList(0);

			const auto value = linkedList.PopBack();
			Assert::IsTrue(value == 0);
			Assert::IsTrue(linkedList.Size() == 0);
			Assert::IsTrue(linkedList.Head() == nullptr);
			Assert::IsTrue(linkedList.Tail() == nullptr);
		}

		TEST_METHOD(Test_PopBack_With_Multiple_Elements)
		{
			Set linkedList(0, 1, 2, 3);

			const auto value = linkedList.PopBack();
			Assert::IsTrue(value == 3);
			Assert::IsTrue(linkedList.Size() == 3);
			Assert::IsTrue(linkedList.Tail()->Value == 2);
		}

		TEST_METHOD(Test_PopFront_With_No_Elements)
		{
			try
			{
				Set linkedList;
				const auto _ = linkedList.PopFront();

				Assert::Fail();
			}
			catch (const InvalidOperationException&)
			{
			}
		}

		TEST_METHOD(Test_PopFront_With_One_Element)
		{
			Set linkedList(0);

			const auto value = linkedList.PopFront();
			Assert::IsTrue(value == 0);
			Assert::IsTrue(linkedList.Size() == 0);
			Assert::IsTrue(linkedList.Head() == nullptr);
			Assert::IsTrue(linkedList.Tail() == nullptr);
		}

		TEST_METHOD(Test_PopFront_With_Multiple_Elements)
		{
			Set linkedList(0, 1, 2, 3);

			const auto value = linkedList.PopFront();
			Assert::IsTrue(value == 0);
			Assert::IsTrue(linkedList.Size() == 3);
			Assert::IsTrue(linkedList.Head()->Value == 1);
		}

		TEST_METHOD(Test_Remove_With_No_Elements)
		{
			Set linkedList;

			const auto result = linkedList.Remove(0);
			Assert::IsTrue(!result);
		}

		TEST_METHOD(Test_Remove_With_No_Matching_Elements)
		{
			Set linkedList(0, 1, 2, 3);

			const auto result = linkedList.Remove(5);
			Assert::IsTrue(!result);
			Assert::IsTrue(linkedList.Size() == 4);
			Assert::IsTrue(linkedList.Head()->Value == 0);
		}

		TEST_METHOD(Test_Remove_Valid_Head)
		{
			Set linkedList(0, 1, 2, 3);

			const auto result = linkedList.Remove(0);
			Assert::IsTrue(result);
			Assert::IsTrue(linkedList.Size() == 3);
			Assert::IsTrue(linkedList.Head()->Value == 1);
		}

		TEST_METHOD(Test_Remove_Valid_Tail)
		{
			Set linkedList(0, 1, 2, 3);

			const auto result = linkedList.Remove(3);
			Assert::IsTrue(result);
			Assert::IsTrue(linkedList.Size() == 3);
			Assert::IsTrue(linkedList.Tail()->Value == 2);
		}

		TEST_METHOD(Test_Clear_Empty_List)
		{
			Set linkedList(0, 1, 2, 3);

			linkedList.Clear();
			Assert::IsTrue(linkedList.Size() == 0);
			Assert::IsTrue(linkedList.Head() == nullptr);
			Assert::IsTrue(linkedList.Tail() == nullptr);
		}

		TEST_METHOD(Test_Clear_Allocated_List)
		{
			Set linkedList(0, 1, 2, 3);

			linkedList.Clear();
			Assert::IsTrue(linkedList.Size() == 0);
			Assert::IsTrue(linkedList.Head() == nullptr);
			Assert::IsTrue(linkedList.Tail() == nullptr);
		}

		TEST_METHOD(Test_Copy_Operator_Unallocated_List)
		{
			Set linkedList{};

			linkedList = TestList;

			Assert::IsTrue(linkedList.Size() == TestList.Size());
			Assert::IsTrue(linkedList.Head()->Value == TestList.Head()->Value);
			Assert::IsTrue(linkedList.Tail()->Value == TestList.Tail()->Value);
		}

		TEST_METHOD(Test_Copy_Operator_Allocated_List)
		{
			Set linkedList(0, 1, 2, 3);

			linkedList = TestList;

			Assert::IsTrue(linkedList.Size() == TestList.Size());
			Assert::IsTrue(linkedList.Head()->Value == TestList.Head()->Value);
			Assert::IsTrue(linkedList.Tail()->Value == TestList.Tail()->Value);
		}

		TEST_METHOD(Test_Move_Operator_Unallocated_List)
		{
			Set moveList = TestList;
			const auto size = moveList.Size();
			const auto headPtr = moveList.Head();
			const auto tailPtr = moveList.Tail();
			Set linkedList{};

			linkedList = std::move(moveList);

			Assert::IsTrue(linkedList.Size() == size);
			Assert::IsTrue(linkedList.Head() == headPtr);
			Assert::IsTrue(linkedList.Tail() == tailPtr);
		}

		TEST_METHOD(Test_Move_Operator_Allocated_List)
		{
			Set moveList = TestList;
			const auto size = moveList.Size();
			const auto headPtr = moveList.Head();
			const auto tailPtr = moveList.Tail();
			Set linkedList(0, 1, 2, 3);

			linkedList = std::move(moveList);

			Assert::IsTrue(linkedList.Size() == size);
			Assert::IsTrue(linkedList.Head() == headPtr);
			Assert::IsTrue(linkedList.Tail() == tailPtr);
		}

	private:
		inline static Set TestList;
	};
}
