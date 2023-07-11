#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Collections/Set.hpp"

using namespace Micro;

namespace Micro::Tests
{
	TEST_CLASS(SetTestRunner_Primitive)
	{
	public:
		// Aliases
		using Set = Set<int>;

		// Setup
		TEST_CLASS_INITIALIZE(Initialization)
		{
			new(&TestSet) Set({0, 1, 2, 3, 4});
		}

		// Unit Tests
		TEST_METHOD(Test_Default_Constructor_Initializes_To_Empty_Set)
		{
			Set set;

			Assert::IsTrue(set.Size() == 0);
			Assert::IsTrue(set.Capacity() == 16);
		}

		TEST_METHOD(Test_Copy_Constructor_Copies)
		{
			Set set(TestSet);

			Assert::IsTrue(set.Size() == TestSet.Size());
			Assert::IsTrue(set.Capacity() == TestSet.Capacity());
		}

		TEST_METHOD(Test_Move_Constructor_Moves)
		{
			Set moveSet = TestSet;
			const auto size = moveSet.Size();
			const auto capacity = moveSet.Capacity();
			Set set(std::move(moveSet));

			Assert::IsTrue(set.Size() == size);
			Assert::IsTrue(set.Capacity() == capacity);
		}

		TEST_METHOD(Test_Initializer_List_Constructor_To_Hash)
		{
			constexpr auto size = 5;
			Set set = {0, 1, 2, 3, 4};

			Assert::IsTrue(set.Size() == size);
			Assert::IsTrue(set.Capacity() == 16);
		}

		TEST_METHOD(Test_Reserve_Constructor_With_100)
		{
			constexpr size_t capacity = 100;
			Set set(100);

			Assert::IsTrue(set.Capacity() == capacity);
		}

		TEST_METHOD(Test_Add_No_Elements)
		{
			Set set;

			auto _ = set.Add(3);
			Assert::IsTrue(set.Size() == 1);
			Assert::IsTrue(set.Capacity() == 16);
		}

		TEST_METHOD(Test_Add_Max_Capacity)
		{
			Set set = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

			auto _ = set.Add(16);
			Assert::IsTrue(set.Size() == 17);
			Assert::IsTrue(set.Capacity() == 32);
		}

		TEST_METHOD(Test_Remove_Invalid_Element)
		{
			Set set = {0, 1, 2, 3, 4};

			const auto result = set.Remove(5);
			Assert::IsTrue(set.Size() == 5);
			Assert::IsTrue(!result);
		}

		TEST_METHOD(Test_Remove_Valid_Element)
		{
			Set set = {0, 1, 2, 3, 4};

			const auto result = set.Remove(2);
			Assert::IsTrue(set.Size() == 4);
			Assert::IsTrue(result);
		}

		TEST_METHOD(Test_Overlaps_No_Elements)
		{
			Set set;

			const auto result = set.Overlaps(TestSet);
			Assert::IsTrue(!result);
		}

		TEST_METHOD(Test_Overlaps_Valid_Element)
		{
			Set set = {0};

			const auto result = set.Overlaps(TestSet);
			Assert::IsTrue(result);
		}

		TEST_METHOD(Test_SetEquals_No_Elements)
		{
			Set set;

			const auto result = set.SetEquals(TestSet);
			Assert::IsTrue(!result);
		}

		TEST_METHOD(Test_SetEquals_Exact_Match)
		{
			Set set = {0, 1, 2, 3, 4};

			const auto result = set.SetEquals(TestSet);
			Assert::IsTrue(result);
		}

		TEST_METHOD(Test_Contains_On_Empty_Set)
		{
			Set set;

			const auto result = set.Contains(0);
			Assert::IsTrue(!result);
		}

		TEST_METHOD(Test_Contains_On_Allocated_Set)
		{
			Set set = {0, 1, 2, 3};

			const auto result = set.Contains(2);
			Assert::IsTrue(result);
		}

		TEST_METHOD(Test_IntersectWith_Empty_Set)
		{
			Set set;

			set.IntersectWith(TestSet);
			Assert::IsTrue(set.IsEmpty());
		}

		TEST_METHOD(Test_IntersectWith_Invalid_Set)
		{
			Set set = {5, 6, 7};

			set.IntersectWith(TestSet);
			Assert::IsTrue(set.IsEmpty());
		}

		TEST_METHOD(Test_IntersectWith_Valid_Set)
		{
			Set set = {0, 1, 5, 6, 7};

			set.IntersectWith(TestSet);
			Assert::IsTrue(set.SetEquals({0, 1}));
			Assert::IsTrue(set.Size() == 2);
		}

		TEST_METHOD(Test_UnionWith_Empty_Set)
		{
			Set set;

			set.UnionWith(TestSet);
			Assert::IsTrue(set.SetEquals({0, 1, 2, 3, 4}));
		}

		TEST_METHOD(Test_UnionWith_Valid_Set)
		{
			Set set = {5, 6, 7};

			set.UnionWith(TestSet);
			Assert::IsTrue(set.SetEquals({0, 1, 2, 3, 4, 5, 6, 7}));
			Assert::IsTrue(set.Size() == 8);
		}

		TEST_METHOD(Test_Clear_Empty_Set)
		{
			Set set;

			set.Clear();
			Assert::IsTrue(set.Size() == 0);
		}

		TEST_METHOD(Test_Clear_Allocated_Set)
		{
			Set set = {0, 1, 2, 3};

			set.Clear();
			Assert::IsTrue(set.Size() == 0);
		}

		TEST_METHOD(Test_Copy_Operator_Unallocated_Set)
		{
			Set set{};

			set = TestSet;

			Assert::IsTrue(set.Size() == TestSet.Size());
			Assert::IsTrue(set.Capacity() == TestSet.Capacity());
		}

		TEST_METHOD(Test_Copy_Operator_Allocated_Set)
		{
			Set set = {0, 1, 2, 3};

			set = TestSet;

			Assert::IsTrue(set.Size() == TestSet.Size());
			Assert::IsTrue(set.Capacity() == TestSet.Capacity());
		}

		TEST_METHOD(Test_Move_Operator_Unallocated_Set)
		{
			Set moveSet = TestSet;
			const auto size = moveSet.Size();
			const auto capacity = moveSet.Capacity();
			Set set{};

			set = std::move(moveSet);

			Assert::IsTrue(set.Size() == size);
			Assert::IsTrue(set.Capacity() == capacity);
		}

		TEST_METHOD(Test_Move_Operator_Allocated_Set)
		{
			Set moveSet = TestSet;
			const auto size = moveSet.Size();
			const auto capacity = moveSet.Capacity();
			Set set = {0, 1, 2, 3};

			set = std::move(moveSet);

			Assert::IsTrue(set.Size() == size);
			Assert::IsTrue(set.Capacity() == capacity);
		}

	private:
		inline static Set TestSet;
	};
}
